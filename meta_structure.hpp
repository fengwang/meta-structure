#ifndef META_STRUCTURE_HPP_INCLUDED_ASDLJOP4IJALKJASLKJDASFLKSDJSADFKJASDFOKJSDF
#define META_STRUCTURE_HPP_INCLUDED_ASDLJOP4IJALKJASLKJDASFLKSDJSADFKJASDFOKJSDF

#include <algorithm>
#include <type_traits>
#include <iostream>
#include <string>

namespace meta
{

template< unsigned long N>
struct fixed_string
{
    char data[N + 1] = {};
    constexpr fixed_string(const char (&foo)[N + 1]) { std::copy_n(foo, N + 1, data); }
    auto operator<=>(const fixed_string&) const = default;
    constexpr operator std::string() const noexcept { return std::string(data); }
};//struct fixed_string

template< unsigned long M, unsigned long N >
constexpr bool operator == (const fixed_string<M>&, const fixed_string<N>&) noexcept
{
    return false;
}

template <unsigned long N>
fixed_string(const char (&str)[N]) -> fixed_string<N - 1>;

template< fixed_string tag_, typename T >
struct member
{
    T value_;

    static constexpr auto tag() noexcept {return tag_; }
    constexpr auto value() const noexcept {return value_; }
};//struct member

template< fixed_string tag_, typename T >
constexpr member<tag_, T> make_member(T value) noexcept
{
    return member<tag_, T>{value};
}

template< typename T >
struct is_member : std::false_type {};

template< fixed_string tag_, typename T >
struct is_member< member<tag_, T> > : std::true_type {};

template< typename T >
constexpr bool is_member_v = is_member<T>::value;

template< typename T >
concept Member = is_member_v<T>;


template< typename S >
struct structure
{
    S s_;

    template< typename F >
    constexpr auto operator()(F && function) const noexcept { return s_( std::forward<F>(function) ); }
};//struct structure

template< typename T >
struct is_structure : std::false_type {};

template< typename T >
struct is_structure< structure<T> > : std::true_type {};

template< typename T >
constexpr bool is_structure_v = is_structure<T>::value;

template< typename T >
concept Structure = is_structure_v<T>;

///
/// @brief CREATE a meta structure with a fixed number of members
/// Example:
/// constexpr auto m1 = make_member<"one">(1);
/// constexpr auto m2 = make_member<"two">(2UL);
/// constexpr auto m3 = make_member<"three">(2.0f);
/// constexpr auto s = create_struct( m1, m2, m3 );
///
template< Member ... Members >
constexpr auto create_struct(Members const& ... members) noexcept
{
    return structure{ [=]<typename F>( F&& function ) noexcept { return std::forward<F>(function)( members... ); } };
}

///
/// @brief concatenate_struct two meta structures
/// Example:
/// constexpr auto s1 = create_struct( m1, m2 );
/// constexpr auto s2 = create_struct( m3 );
/// constexpr auto s3 = concatenate_struct( s1, s2 );
/// constexpr auto s4 = concatenate_struct( s1, s2, s3 );
///
template< Structure S1, Structure S2 >
constexpr auto concatenate_struct(S1 const& structure1, S2 const& structure2) noexcept
{
    return structure1( [=]<Member ... MS>(MS const& ... members) noexcept
    {
        return structure2( [=]<Member ... MT>(MT const& ... members2) noexcept { return create_struct(members..., members2...); }); }
    );
}

template< Structure S,  Structure ... SS >
constexpr auto concatenate_struct(S const& s, SS const& ... ss ) noexcept
{
    if constexpr (sizeof...(SS) == 0 ) return s;
    else return concatenate_struct(s, concatenate_struct(ss...));
}

///
/// @brief READ a field from a meta structure.
/// Example:
/// constexpr auto s = create_struct( m1, m2, m3 );
/// constexpr auto v = read_struct<"one">( s );
///
template< fixed_string tag_, Structure S >
constexpr auto read_struct( S const& structure ) noexcept
{
    return structure( [=]<Member M, Member ... MS>( M const& member1, MS const& ... members ) noexcept
    {
        if constexpr ( M::tag() == tag_ )
            return member1.value();
        else
            return read_struct<tag_>( create_struct(members...) );
    } );
}

///
/// @brief UPDATE a field in a meta structure.
/// Example:
/// constexpr auto s = create_struct( m1, m2, m3 );
/// constexpr auto t = update_struct<"one">( s, 1.0f );
///
template< fixed_string tag_, Structure S, typename T >
constexpr auto update_struct( S const& structure, T const& value ) noexcept
{
    return structure( [=]<Member M, Member ... MS>( M const& member1, MS const& ... members ) noexcept
    {
        if constexpr ( M::tag() == tag_ )
            return create_struct( make_member<tag_>( value ), members... );
        else
            return  concatenate_struct( create_struct(member1), update_struct<tag_>( create_struct(members...), value ) );
    } );
}

///
/// @brief DELETE a field in a meta structure.
/// Example:
/// constexpr auto s = create_struct( m1, m2, m3 );
/// constexpr auto t = delete_struct<"one">( s );
///
template< fixed_string tag_, Structure S >
constexpr auto delete_struct( S const& structure ) noexcept
{
    return structure( [=]<Member M, Member ... MS>( M const& member1, MS const& ... members ) noexcept
    {
        if constexpr ( M::tag() == tag_ )
            return create_struct( members... );
        else
            return concatenate_struct( create_struct( member1 ), delete_struct<tag_>( create_struct(members...) ) );
    } );
}

///
/// @breif MAP a function to each field in the meta structure.
/// EXAMPLE:
/// constexpr auto s = create_struct( m1, m2, m3 );
/// constexpr auto t = map_struct( s, []<Member M>(M const& member) { std::cout << static_cast<std::string>(M::tag()) << ": " << member.value() << std::endl; return member.value(); } );
///
template< Structure S, typename F >
constexpr auto map_struct( S const& structure, F && function ) noexcept
{
    return structure( [=]<Member M, Member ... MS>( M const& member1, MS const& ... members ) noexcept
    {
        if constexpr( sizeof...(MS) == 0 )
            return create_struct( make_member<M::tag()>(std::forward<F>(function)(member1)) );
        else
            return concatenate_struct ( create_struct( make_member<M::tag()>(std::forward<F>(function)(member1)) ), map_struct( create_struct(members...), std::forward<F>(function) ) );
    } );
}

}//namespace meta

#endif//META_STRUCTURE_HPP_INCLUDED_ASDLJOP4IJALKJASLKJDASFLKSDJSADFKJASDFOKJSDF
