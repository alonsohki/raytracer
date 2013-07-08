//
// FILE:        CPool.h
// LICENSE:     The MIT license
// PURPOUSE:    Pooled memory allocation
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
// NOTES:       From https://github.com/ryden/graphics/blob/master/Graficos/source/CPool.h
//

#pragma once

template < class T > class CPool;

#include <malloc.h>
#ifdef _OPENMP
#   include <omp.h>
#endif

template < class T >
class CPool
{
private:
    struct SPoolNode
    {
        void*       p;
        SPoolNode*  pNext;
        SPoolNode*  pNextFree;
    };

public:
    inline          CPool           ( unsigned int uiBucketSize = 1024 )
        : m_uiBucketSize ( uiBucketSize ),
          m_pBuckets ( 0 ),
          m_pFreeNodes ( 0 )
    {
#ifdef _OPENMP
        omp_init_lock(&m_lock);
#endif
        CreateBucket ( );
    }

    inline          ~CPool          ( )
    {
#ifdef _OPENMP
        omp_destroy_lock(&m_lock);
#endif
        SPoolNode* pNextBucket;

        for ( SPoolNode* pCurBucket = m_pBuckets;
              pCurBucket != 0;
              pCurBucket = pNextBucket )
        {
            pNextBucket = pCurBucket->pNext;
            free ( pCurBucket );
        }
    }

    inline void     CreateBucket    ( )
    {
        typedef unsigned char BYTE;

        /* Reservamos toda la memoria en un sólo malloc para minimizar al máximo la fragmentación */
        SPoolNode* pNewNode = ( SPoolNode * ) malloc ( ( sizeof ( SPoolNode ) * ( m_uiBucketSize + 1 ) ) +
                                                       ( sizeof ( T ) * m_uiBucketSize ) );
        T* pElements = ( T * ) ( (BYTE *)pNewNode + sizeof ( SPoolNode ) * ( m_uiBucketSize + 1 ) );
        SPoolNode* pNodes = pNewNode + 1;

        /* Enlazamos este nuevo bucket de nodos */
        pNewNode->p = 0;
        pNewNode->pNext = m_pBuckets;
        pNewNode->pNextFree = 0;
        m_pBuckets = pNewNode;

        /* Creamos la lista de nodos */
        for ( unsigned int i = 0; i < m_uiBucketSize; ++i )
        {
            pNodes->p = &pElements [ m_uiBucketSize - i - 1 ];
            pNodes->pNextFree = m_pFreeNodes;
            pNodes->pNext = static_cast < SPoolNode * > ( pNewNode->p );
            pNewNode->p = pNodes;
            m_pFreeNodes = pNodes;

            ++pNodes;
        }
    }

    inline T*       Alloc           ()
    {
#ifdef _OPENMP
        omp_set_lock(&m_lock);
#endif

        if ( ! m_pFreeNodes )
        {
            CreateBucket ( );
        }

        T* pRet = new ( m_pFreeNodes->p ) T ();
        m_pFreeNodes = m_pFreeNodes->pNextFree;

#ifdef _OPENMP
        omp_unset_lock(&m_lock);
#endif

        return pRet;
    }

    inline void     Free            ( T* p )
    {
#ifdef _OPENPM
        omp_set_lock(&m_lock);
#endif

        for ( SPoolNode* pCurrentBucket = m_pBuckets;
              pCurrentBucket != 0;
              pCurrentBucket = pCurrentBucket->pNext )
        {
            /* Como todos los elementos de este bucket se reservaron secuencialmente,
             * Podemos calcular diréctamente la posición dentro del bucket del nodo
             * que hace referencia al elemento que vamos a liberar.
             */
            T* pElementsBase = static_cast < T * > ( static_cast < SPoolNode * > ( pCurrentBucket->p )->p );
            if ( p >= pElementsBase && p < pElementsBase + m_uiBucketSize )
            {
                unsigned int uiPosition = static_cast < unsigned int > ( p - pElementsBase );
                SPoolNode* pNode = pCurrentBucket + m_uiBucketSize - uiPosition;
                ((T*)(pNode->p))->~T();

                pNode->pNextFree = m_pFreeNodes;
                m_pFreeNodes = pNode;
                break;
            }
        }

#ifdef _OPENMP
        omp_unset_lock(&m_lock);
#endif
    }

private:
    unsigned int        m_uiBucketSize;
    SPoolNode*          m_pBuckets;
    SPoolNode*          m_pFreeNodes;
#ifdef _OPENMP
    omp_lock_t          m_lock;
#endif
};