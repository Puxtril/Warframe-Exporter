// SPDX-FileCopyrightText: 2023 Stephan Lachnit <stephanlachnit@debian.org>
// SPDX-License-Identifier: MIT

#pragma once

#include <algorithm>
#include <cstdlib>
#include <ios>
#include <streambuf>

// std::streambuf implementation in memory with efficient resizing and possibility to keep the buffer after exit
template<std::streamsize BLOCK_SIZE = 1024>
class membuf : public std::streambuf {
public:
    using streambuf = std::streambuf;
    using char_type = typename streambuf::char_type;
    using int_type = typename streambuf::int_type;
    using traits_type = typename streambuf::traits_type;
    using pos_type = typename streambuf::pos_type;
    using off_type = typename streambuf::off_type;

    membuf() { set_capacity(BLOCK_SIZE); }

    virtual ~membuf() { if (free_in_destructor_) { std::free(memptr_); } }

    // If set to false, does not free the buffer on destruction, user has to call std::free() itself
    constexpr void free_in_destructor(bool do_free) { free_in_destructor_ = do_free; }

    // Changes the buffer capacity to an exact value
    constexpr void set_capacity(std::streamsize new_size) {
        // minimum size is one byte
        if (new_size <= 0) { throw std::bad_alloc(); }

        // (re-)allocate memory, returns either ptr to memory or nullptr on failure
        void* new_memptr = std::realloc(memptr_, new_size);
        if (new_memptr == nullptr) { throw std::bad_alloc(); }
        memptr_ = static_cast<char_type*>(new_memptr);

        // store currently used size of underlying streambuf
        const std::streamsize current_size = get_size();

        // set pbase, pptr and epptr to new buffer
        streambuf::setp(memptr_, memptr_ + new_size);

        // advance pptr to old position or end of buffer is smaller
        streambuf::pbump(std::min(current_size, new_size));

        // update get area pointers
        const std::streamsize current_getpos = streambuf::gptr() - streambuf::eback();
        streambuf::setg(streambuf::pbase(), streambuf::pbase() + current_getpos, streambuf::pptr());
    }

    // Increase the buffer capacity by additional_size
    constexpr void increase_capacity(std::streamsize additional_size) {
        set_capacity(get_capacity() + additional_size);
    }

    // Increases the buffer capacity by N blocks such that additional_size < N * BLOCK_SIZE
    constexpr void increase_block_capacity(std::streamsize additional_size) {
        const std::streamsize new_blocks = 1 + additional_size / BLOCK_SIZE;
        increase_capacity(new_blocks * BLOCK_SIZE);
    }

    // Shrinks the buffer to the used data
    constexpr void shrink_to_fit() { set_capacity(get_size()); }

    // Returns the current memory pointer of the buffer, can change if data is added
    constexpr void* get_memptr() const { return memptr_; }

    // Returns the current size of the data in the buffer currently in use
    constexpr std::streamsize get_size() const { return streambuf::pptr() - streambuf::pbase(); }

    // Returns the current capacity of the buffer
    constexpr std::streamsize get_capacity() const { return streambuf::epptr() - streambuf::pbase();; }

    // Returns the free capacity of the buffer
    constexpr std::streamsize get_free_capacity() const { return streambuf::epptr() - streambuf::pptr(); }

    // Implements std::basic_streambuf::xsputn
    std::streamsize xsputn(const char_type* s, std::streamsize count) override {
        if (count > get_free_capacity()) {
            // more to write than free capacity, try to increase to at least count for speedup
            try {
                increase_block_capacity(count);
            }
            catch (const std::bad_alloc& error) {
                // fine if throw, streambuf::xsputn will call overflow
            }
        }

        // use streambuf::xsputn, store written chars for get area
        std::streamsize written = streambuf::xsputn(s, count);
        // update streambuf::egptr
        streambuf::setg(streambuf::eback(), streambuf::gptr(), streambuf::pptr());

        return written;
    }

    // Implements std::basic_streambuf::overflow
    int_type overflow(int_type ch) override {
        // try to increase the size by one block
        try {
            increase_block_capacity(sizeof(char_type));
        }
        catch (const std::bad_alloc& error) {
            // size increase did not work, return eof
            return traits_type::eof();
        }
        // put char if not eof
        if (!traits_type::eq_int_type(ch, traits_type::eof())) {
            char_type ch_char = traits_type::to_char_type(ch);
            xsputn(&ch_char, 1);
        }
        return traits_type::not_eof(ch);
    }

    // Implements std::basic_streambuf::showmanyc
    std::streamsize showmanyc() override {
        // never characters in associated character since xsputn automatically sets streambuf::egptr
        return -1;
    }

    // Implements std::basic_streambuf::setbuf
    // Note: either the the buffer needs to be freed by the user via free_in_destructor(false)
    //       or the buffer needs to be allocated via std::malloc so that std::free works
    membuf* setbuf(char_type* s, std::streamsize n) override {
        // free current memory
        std::free(memptr_);
        // update memptr to new character sequence
        memptr_ = s;
        // set pbase, pptr and epptr to new buffer
        streambuf::setp(memptr_, memptr_ + n);

        return this;
    }

private:
    bool free_in_destructor_ {true};
    char_type* memptr_ {nullptr};
};
