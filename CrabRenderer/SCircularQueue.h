#pragma once

/*
 *
 * SCircularQueue.h
 * ======================
 * static circular queue
 * fixed size, no dynamic allocation
 *
 */

#include <array>

namespace crab
{

template<typename Ty, size_t CAPACITY>
class SCircularQueue
{
public:
    SCircularQueue() = default;
    ~SCircularQueue() = default;

    void Push(const Ty& in_data)
    {
        if (IsFull())
        {
            CRAB_ASSERT(false, "SCircularQueue is full");
            return;
        }

        m_data[m_back] = in_data;
        m_back         = (m_back + 1) % CAPACITY;
    }

    void Pop()
    {
        if (IsEmpty())
        {
            CRAB_ASSERT(false, "SCircularQueue is empty");
            return;
        }

        m_front = (m_front + 1) % CAPACITY;
    }

    Ty& Front()
    {
        if (IsEmpty())
        {
            CRAB_ASSERT(false, "SCircularQueue is empty");
            return m_data[m_front];
        }

        return m_data[m_front];
    }
    
    bool IsEmpty() const
    {
        return m_front == m_back;
    }

    bool IsFull() const
    {
        return (m_back + 1) % CAPACITY == m_front;
    }

    size_t Size() const
    {
        return (m_back - m_front + CAPACITY) % CAPACITY;
    }

    size_t Capacity() const
    {
        return CAPACITY;
    }

    void Clear()
    {
        m_front = m_back = 0;
    }

    Ty& operator[](size_t in_index)
    {
        return m_data[(m_front + in_index) % CAPACITY];
    }
    
    const Ty& operator[](size_t in_index) const
    {
        return m_data[(m_front + in_index) % CAPACITY];
    }

    Ty* Data()
    {
        return m_data.data();
    }

    const Ty* Data() const
    {
        return m_data.data();
    }

    Ty* begin()
    {
        return m_data.data();
    }

    Ty* end()
    {
        return m_data.data() + CAPACITY;
    }
    
    const Ty* begin() const
    {
        return m_data.data();
    }

    const Ty* end() const
    {
        return m_data.data() + CAPACITY;
    }

private:
    std::array<Ty, CAPACITY> m_data;

    size_t m_front = 0;
    size_t m_back  = 0;
};

}   // namespace crab