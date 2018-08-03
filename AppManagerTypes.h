#ifndef APPMANAGERTYPES_H
#define APPMANAGERTYPES_H


#include "AppTypes.h"


enum eManagerError
{
    ME_SUCCESS=0,
    ME_INVALID_ID, // given id is incorrect
    ME_INVALID_POOL, // action not supported for object pool type
    ME_PARAMETER_OUT_OF_BOUNDS,
    ME_UNKNOWN_ERROR,
    ME_INVALID_OPERATION
};

enum ePoolType
{
    PT_LAZYCACHED,
    PT_PRECACHED,
    PT_AUTOCNT,
    PT_UNKNOWN
};




template <typename TDerrived, typename T>

class TObjectManager : public TSingleton<TDerrived>
{
public:

    const unsigned int MAX_PRECACHED=  512; // 9 bits
    const unsigned int MAX_LAZY= 1024; // 10 bits
    const unsigned int MAX_AUTO= 4096; // 12 bits - 1

    const unsigned int PRECACHE_BITS = 9;
    const unsigned int LAZY_BITS = 10;
    const unsigned int AUTO_BITS = 12;

    const unsigned int INVALID_ID = 0xFFFFFFFF;

    // used when you want to create an object

    // creates an object and returns the id of the object

    unsigned int createObject(eManagerError& error, ePoolType policy=PT_LAZYCACHED)
    {

        unsigned int rez = INVALID_ID;
        getObject(rez,error,policy);
        return rez;
    }

    T* getObjectForId(unsigned int& id)
    {
        eManagerError err;
        ePoolType pt = getPoolForID(id);
        return getObject(id,err,pt);
    }

    // attempts to create / retrieve an object with an ID
    // provide -1 for guaranteed new allocation
     T* getObject(unsigned int& id,eManagerError& error, ePoolType policy=PT_LAZYCACHED)
    {
        error = ME_SUCCESS;
        if (id==INVALID_ID)
        {
            // allocate
            switch (policy)
            {
                case PT_LAZYCACHED:
                    if (m_vpLazyCached.size() < MAX_LAZY - 1)
                    {
                        id = (m_vpLazyCached.size()+1) << 9;
                        T* ret = new T();
                        m_vpLazyCached.push_back(ret);
                        return ret;
                    }
                break;
                case PT_PRECACHED:
                    error = ME_INVALID_OPERATION;
                    return NULL;
                break;
                case PT_AUTOCNT:
                    {

                        int idx = -1;
                        if (m_vFreeAutoIdx.size()>0)
                        {
                            idx = m_vFreeAutoIdx[m_vFreeAutoIdx.size()-1];
                            m_vFreeAutoIdx.pop_back();

                            if (m_vpAutoCnt[idx]!=0)
                            {
                                error= ME_UNKNOWN_ERROR;
                                return NULL;
                            }


                        } else {
                            if (m_vpAutoCnt.size()>=MAX_AUTO)
                            {
                               error = ME_PARAMETER_OUT_OF_BOUNDS;
                               return NULL;
                            }

                            m_vpAutoCnt.push_back(0);
                            idx = m_vpAutoCnt.size()-1;
                        }


                            id = (idx+1) << (LAZY_BITS+PRECACHE_BITS);
                            void *p = malloc(sizeof(T)+sizeof(int));

                            int* cnt =(int*) p;
                            *cnt =1;
                            T* ptr = new (++cnt) T();
                            m_vpAutoCnt[idx]=p;

                            return ptr;
                        }
                break;

                case PT_UNKNOWN:
                default:
                break;
            }

        } else {

            // search
           ePoolType pType= getPoolForID(id);
           unsigned int idx = getIdxForId(id,pType);
           if (idx ==(unsigned int) -1 )
           {
               error = ME_INVALID_POOL;
               return NULL;
           }
           switch (pType) {
           case PT_LAZYCACHED:
                if (idx >= m_vpLazyCached.size())
                {
                    error = ME_PARAMETER_OUT_OF_BOUNDS;
                    return NULL;

                } else {
                    return m_vpLazyCached[idx];
                }
               break;
           case PT_PRECACHED:
               if (idx >= m_nPrecachedCnt)
               {
                   error = ME_PARAMETER_OUT_OF_BOUNDS;
                   return NULL;
               } else {
                   unsigned char* p =(unsigned char*) m_pPrecachedPool;
                   return (T*) (p + idx*sizeof(T));
               }
               break;
           case PT_AUTOCNT:
               if (idx >= m_vpAutoCnt.size()){
                    error = ME_PARAMETER_OUT_OF_BOUNDS;
                    return NULL;
               } else {
                   if (m_vpAutoCnt[idx] == 0)
                       return NULL;
                   int* ptr =(int*) m_vpAutoCnt[idx];
                   (*ptr)++;

                   return (T*) (++ptr);
               }
               break;
           case PT_UNKNOWN:
           default:
               break;
           }

        }
        return NULL;
    }

    // releases an object according to the id
    // only valid for Auto Stored
    eManagerError releaseObject(unsigned int id)
    {

        if (getPoolForID(id)==PT_AUTOCNT)
        {
            unsigned int idx =getIdxForId(id,PT_AUTOCNT);

            if (idx < m_vpAutoCnt.size())
            {
                if (m_vpAutoCnt[idx]!=0)
                {
                    int* cnt =(int*)m_vpAutoCnt[idx];
                    (*cnt)--;
                    if (*cnt <=0)
                    {
                        cnt++;
                        T* ptr =(T*) cnt;
                        ptr->~T();
                        free(m_vpAutoCnt[idx]);
                        m_vpAutoCnt[idx]=0;
                        m_vFreeAutoIdx.push_back(idx);
                    }
                } else {
                }
            } else {

                return ME_PARAMETER_OUT_OF_BOUNDS;
            }
        } else {
            return ME_INVALID_OPERATION;
        }
        return ME_SUCCESS;
    }


    // clears all memory used by the manager
    void destroyAll() {
        destroyPool(PT_LAZYCACHED);
        destroyPool(PT_PRECACHED);
        destroyPool(PT_AUTOCNT);
    }

    // clears a pool
    void destroyPool(ePoolType policy){
        switch (policy)
        {
            case PT_LAZYCACHED:
                {
                    for (unsigned int i=0;i<m_vpLazyCached.size();i++)
                    {
                        delete m_vpLazyCached[i];
                    }
                    m_vpLazyCached.clear();
                }
                break;
            case PT_PRECACHED:
                {
                    T* ptr = (T*) m_pPrecachedPool;
                    for (unsigned int i=0;i<m_nPrecachedCnt;i++)
                    {
                        ptr->~T();
                        ptr++;
                    }
                    free(m_pPrecachedPool);
                    m_nPrecachedCnt=0;
                 }
                break;

            case PT_AUTOCNT:
                {
                    for (unsigned int i=0;i<m_vpAutoCnt.size();i++)
                    {
                        int* cnt =(int*) m_vpAutoCnt[i];
                        if (cnt!=0)
                        {
                            *cnt=0;
                            cnt++;
                            T* ptr = (T*) cnt;
                            ptr->~T();
                            free(m_vpAutoCnt[i]);
                            m_vpAutoCnt[i]=0;
                        }
                    }
                    m_vpAutoCnt.clear();
                }
                break;
            default:
                break;
        }
    }

    void setComparator(std::function<bool (T*)> fct)
    {
        m_isSame = fct;
    }

    ePoolType getPoolForID(unsigned int objId)
    {
        if (objId == INVALID_ID)
            return PT_UNKNOWN;
        if (objId < MAX_PRECACHED)
        {
            return PT_PRECACHED;
        } else {
            if ((objId >> 9) < MAX_LAZY)
            {
                return PT_LAZYCACHED;
            } else {
                return PT_AUTOCNT;
            }
        }
        return PT_UNKNOWN;
    }

    eManagerError createPrecachePool(unsigned int size)
    {
        if (size >= MAX_PRECACHED)
        {
            return ME_PARAMETER_OUT_OF_BOUNDS;
        } else {

            m_pPrecachedPool = malloc(size * sizeof(T));
            if (m_pPrecachedPool == 0)
            {
                return ME_UNKNOWN_ERROR;
            } else {
                m_nPrecachedCnt=size;
                memset(m_pPrecachedPool,0,size*sizeof(T));
                T* ptr =(T*) m_pPrecachedPool;
                for (unsigned int i=0;i<m_nPrecachedCnt;i++)
                {
                    new (ptr) T();
                    ptr++;
                }
            }

        }
        return ME_SUCCESS;
    }

    inline unsigned int getAutoCnt() { return m_vpAutoCnt.size() - m_vFreeAutoIdx.size();}
    inline unsigned int getLazyCnt() {return m_vpLazyCached.size();}
    inline unsigned int getPrecacheCnt() {return m_vpLazyCached.size();}


protected:
    TObjectManager():
        m_nPrecachedCnt(0),
        m_pPrecachedPool(nullptr)

    {

    }

    ~TObjectManager()
    {
        // clear pools
        destroyAll();
    }

    unsigned int getIdxForId(int id, ePoolType policy)
    {
        switch (policy)
        {
            case PT_LAZYCACHED:
                    return ((id & ~( ~(0xffffffff << PRECACHE_BITS) | ~(0xffffffff >> AUTO_BITS)  ) ) >> PRECACHE_BITS) -1;
                break;
            case PT_PRECACHED:
                    return (id & ~(0xfffffffff << PRECACHE_BITS));
                break;
            case PT_AUTOCNT:
                    return (id >>(PRECACHE_BITS+LAZY_BITS) )-1 ;
                break;
            case PT_UNKNOWN:
            default:
            break;

        }
        return -1;
    }

    unsigned int m_nPrecachedCnt;

    void* m_pPrecachedPool; // limited number of preallocated objects

    std::vector<T*> m_vpLazyCached;
    std::vector<void *> m_vpAutoCnt;
    std::vector<int> m_vFreeAutoIdx;



    std::function<bool(T*)> m_isSame;

};


template <typename T>
class TObjectManagerAlone : public TObjectManager<TObjectManagerAlone<T> ,T>
{

};

#endif // APPMANAGERTYPES_H
