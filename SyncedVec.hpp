#ifndef _SYNCED_VEC_HPP
#define _SYNCED_VEC_HPP

#include <vector>
#include <mutex>
#include <memory>
#include <exception>

// A thread-safe version of a std::vector<T>
//
// Note that SyncedVec objects cannot be passed by const reference or copied due to 
// the private std::mutex member, m_mutex
//
// Note that this class does not necessarily provide all functionality a std::vector does
template <typename T>
class SyncedVec {
public:

    // ----------------------------- CONSTRUCTORS ---------------------------------------- //

    SyncedVec() : m_vec(), m_mutex() {}

    // No copy construction allowed
    SyncedVec(SyncedVec const&) = delete;

    // ------------------------ PUBLIC MEMBER FUNCTIONS ---------------------------------- //

    // No copy assignment allowed
    SyncedVec& operator=(SyncedVec const&) = delete;

    // Returns the size of m_vec
    uint32_t size() const {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_vec.size();
    }

    // Test whether or not m_vec is empty
    bool empty() const {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_vec.empty();
    }

    // Obtains the item in m_vec[i] and returns a pointer to that item
    // Returns nullptr if m_vec is empty or if i is out of range
    std::shared_ptr<T> operator[](int const & i) {
        std::lock_guard<std::mutex> lock(m_mutex);

        if (i < 0 || i >= m_vec.size()) return nullptr;

        try {
           return std::make_shared<T>(m_vec.at(i));
        } catch(std::exception e) {
            return nullptr;
        }
    }

private:

    // ------------------------ VARIABLES AND CONSTANTS ---------------------------------- //

    std::vector<T> m_vec;

    std::mutex m_mutex;

    // ------------------------ PRIVATE MEMBER FUNCTIONS ---------------------------------- //
};

#endif
