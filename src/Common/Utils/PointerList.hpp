/*!
 * \file
 * \brief Définition de la classe PointerList.
 */

#ifndef UTILS_POINTER_LIST_HPP
#define UTILS_POINTER_LIST_HPP

#include <bits/allocator.h>
#include <cstddef>
#include <list>

namespace utils
{
    /*!
     * \brief This list implements the standard list class to manage some
     * pointer and their destruction.
     *
     * When the list is destroyed, all the registered pointer are deleted.
     */
    template < typename _Tp, typename _Alloc = std::allocator < _Tp > >
            class PointerList : public std::list < _Tp, _Alloc>
    {
    public:

        /*!
         * \brief Type of the associed values.
         */
        typedef _Tp value_type;

        /*!
         * \brief Type of the allocator associed to _Tp.
         */
        typedef _Alloc allocator_type;

        /*!
         * \brief Type for the size of this list.
         * \todo Check the use of this type
         */
        typedef size_t size_type;

        /*!
         * \brief Type for the iterators of this list.
         */
        typedef std::_List_iterator < _Tp > iterator;

        /*!
         * \brief Type for the constant iterators of this list.
         */
        typedef std::_List_const_iterator < _Tp > const_iterator;

        /*!
         * \brief Default constructor creates no elements.
         * \param a
         *        Allocator.
         */
        explicit PointerList(const allocator_type & a = allocator_type())
        : std::list < value_type, allocator_type > (a)
        {
        }

        /*!
         * \brief Create a list with copies of an exemplar element.
         * \param n
         *        The number of elements to initially create.
         * \param value
         *        An element to copy.
         * \param a
         *        Allocator.
         *
         *  This constructor fills the list with n copies of value.
         */
        explicit PointerList(size_type n, const value_type & value = value_type(), const allocator_type & a = allocator_type())
        : std::list < value_type, allocator_type > (n, value, a)
        {
        }

        /*!
         * \brief List copy constructor.
         * \param x
         *        A list of identical element and allocator types.
         *
         * The newly-created list uses a copy of the allocation object used by x.
         */
        PointerList(const PointerList & x)
        : std::list < value_type, allocator_type > (x.get_allocator())
        {
            this->insert(this->begin(), x.begin(), x.end());
        }

        /*!
         * \brief Builds a list from a range.
         * \param first
         *        An input iterator.
         * \param last
         *        An input iterator.
         * \param a
         *        Allocator.
         *
         * Create a list consisting of copies of the elements from [first,last).
         * This is linear in N (where N is distance(first,last)).
         */
        template < typename _InputIterator > PointerList(_InputIterator first, _InputIterator last, const allocator_type & a = allocator_type())
        : std::list < value_type, allocator_type > (a)
        {
            this->insert(begin(), first, last);
        }

        /*!
         * \brief Class destructor.
         */
        virtual ~PointerList()
        {
            // Must not delete items on list
        }

        /*!
         *  \brief Returns a read/write iterator that points to the first element in the
         *  %list.  Iteration is done in ordinary element order.
         */
        iterator begin()
        {
            return iterator(this->_M_impl._M_node._M_next);
        }

        /*!
         *  \brief Returns a read-only (constant) iterator that points to the
         *  first element in the %list.  Iteration is done in ordinary
         *  element order.
         */
        const_iterator begin() const
        {
            return const_iterator(this->_M_impl._M_node._M_next);
        }

        /*!
         *  \brief Returns a read/write iterator that points one past the last
         *  element in the %list.  Iteration is done in ordinary element
         *  order.
         */
        iterator end()
        {
            return iterator(&this->_M_impl._M_node);
        }

        /*!
         *  \brief Returns a read-only (constant) iterator that points one past
         *  the last element in the %list.  Iteration is done in ordinary
         *  element order.
         */
        const_iterator end() const
        {
            return const_iterator(&this->_M_impl._M_node);
        }
    };
};

#endif
