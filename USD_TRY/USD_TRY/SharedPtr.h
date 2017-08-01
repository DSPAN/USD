#ifndef _SharedPtr_H_
#define _SharedPtr_H_

template<class T> class SharedPtr {
	protected:
		T* pRep;           
		unsigned int* pUseCount; 
	public:
		/// OGRE_AUTO_SHARED_MUTEX            
			SharedPtr() : pRep(0), pUseCount(0)
		{
		}

			SharedPtr(int tempnullptr)
				: pRep(0), pUseCount(0)
			{
			}

		template< class Y>
			explicit SharedPtr(Y* rep) : pRep(rep), pUseCount(new unsigned int(1))
			{
			}



			SharedPtr(const SharedPtr& r)
				: pRep(0), pUseCount(0)
			{
				{
					pRep = r.pRep; //注意下与后面的不同
					pUseCount = r.pUseCount;
					// Handle zero pointer gracefully to manage STL containers
					if(pUseCount)
					{
						++(*pUseCount); 
					}
				}
			}

			SharedPtr& operator=(const SharedPtr& r) {
				if (pRep == r.pRep)
					return *this;
				SharedPtr<T> tmp(r);
				swap(tmp);
				return *this;
			}

			bool operator==(const SharedPtr& r) {
				if (pRep == r.pRep)
					return true;
				return false;
			}
			template< class Y>
			SharedPtr(const SharedPtr<Y>& r)
				: pRep(0), pUseCount(0)
			{
				{
					pRep = r.getPointer(); 
					pUseCount = r.useCountPointer();
					if(pUseCount)
					{
						++(*pUseCount);
					}
				}
			}
			template< class Y>
			SharedPtr& operator=(const SharedPtr<Y>& r) {
				if (pRep == r.pRep)
					return *this;
				SharedPtr<T> tmp(r);
				swap(tmp);
				return *this;
			}
			virtual ~SharedPtr() {
				release();
			}
			inline T& operator*() const { assert(pRep); return *pRep; }
			inline T* operator->() const { assert(pRep); return pRep; }
			//在用的时候检查pRep的合法性
			inline T* get() const { return pRep; }
			void bind(T* rep) {
				assert(!pRep && !pUseCount);
				//OGRE_NEW_AUTO_SHARED_MUTEX
				//OGRE_LOCK_AUTO_SHARED_MUTEX
					pUseCount = new unsigned int(1);
				pRep = rep;
			}
			void addRef()
			{
				++(*pUseCount);
			}
			inline bool unique() const {/* OGRE_LOCK_AUTO_SHARED_MUTEX*/ assert(pUseCount); return *pUseCount == 1; }
			inline unsigned int useCount() const {/* OGRE_LOCK_AUTO_SHARED_MUTEX */assert(pUseCount); return *pUseCount; }
			inline unsigned int* useCountPointer() const { return pUseCount; }
			inline T* getPointer() const { return pRep; }
			inline bool isNull(void) const { return pRep == 0; }
			inline void setNull(void) {
				if (pRep)
				{
					// can't scope lock mutex before release incase deleted
					release();
					pRep = 0;
					pUseCount = 0;
				}
			}

	protected:

		inline void release(void)
		{
			bool destroyThis = false;
			//OGRE_MUTEX_CONDITIONAL(OGRE_AUTO_MUTEX_NAME)
			{
				//OGRE_LOCK_AUTO_SHARED_MUTEX
					if (pUseCount)
					{
						if (--(*pUseCount) == 0)
						{
							destroyThis = true;
						}
					}
			}
			if (destroyThis)
				destroy();

			//OGRE_SET_AUTO_SHARED_MUTEX_NULL
		}

		virtual void destroy(void)
		{
			delete pRep;
			delete pUseCount;
			//OGRE_DELETE_AUTO_SHARED_MUTEX
		}

		virtual void swap(SharedPtr<T> &other)
		{
			std::swap(pRep, other.pRep);
			std::swap(pUseCount, other.pUseCount);
// #if OGRE_THREAD_SUPPORT
// 			std::swap(OGRE_AUTO_MUTEX_NAME, other.OGRE_AUTO_MUTEX_NAME);
// #endif
		}
	};

	template<class T, class U> inline bool operator==(SharedPtr<T> const& a, SharedPtr<U> const& b)
	{
		return a.get() == b.get();
	}

	template<class T, class U> inline bool operator!=(SharedPtr<T> const& a, SharedPtr<U> const& b)
	{
		return a.get() != b.get();
	}

#endif