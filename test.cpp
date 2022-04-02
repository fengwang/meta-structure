#include "./meta_structure.hpp"

#include <iostream>
#include <string>


// create
void test_create()
{
    std::cout << std::endl << "test CREATE" << std::endl;
    using namespace meta;

    auto constexpr s = create_struct( make_member<"one">( 1 ), make_member<"two">( 2UL ), make_member<"three">( 3.0f ) );
    constexpr auto print_func =  []<Member M>(M const& member) { std::cout << static_cast<std::string>(M::tag()) << ": " << member.value() << std::endl; return member.value(); };
    map_struct( s, print_func );

    std::cout << "---------------------------------------------------------\n";
}

// read
void test_read()
{
    std::cout << std::endl << "test READ" << std::endl;
    using namespace meta;

    auto constexpr s = create_struct( make_member<"one">( 1 ), make_member<"two">( 2UL ), make_member<"three">( 3.0f ) );

    std::cout << "read one: " << read_struct<"one">( s ) << std::endl;
    std::cout << "read two: " << read_struct<"two">( s ) << std::endl;
    std::cout << "read three: " << read_struct<"three">( s ) << std::endl;
    std::cout << "---------------------------------------------------------\n";
}

// update
void test_update()
{
    std::cout << std::endl << "test UPDATE" << std::endl;
    using namespace meta;

    auto constexpr s = create_struct( make_member<"one">( 1 ), make_member<"two">( 2UL ), make_member<"three">( 3.0f ) );
    constexpr auto print_func =  []<Member M>(M const& member) { std::cout << static_cast<std::string>(M::tag()) << ": " << member.value() << std::endl; return member.value(); };
    std::cout << "created structure:\n" << std::endl;
    map_struct( s, print_func );

    std::cout << "\nafter update one with 1.01(double), the structure becomes:" << std::endl;
    constexpr auto t = update_struct<"one">( s, 1.01 );
    map_struct( t, print_func );

    std::cout << "\nafter update two with '2.0'(float), the structure becomes:" << std::endl;
    constexpr auto u = update_struct<"two">( t, 2.0f );
    map_struct( u, print_func );

    std::cout << "\nafter update three with 'c'(char), the structure becomes:" << std::endl;
    constexpr auto v = update_struct<"three">( u, 'c' );
    map_struct( v, print_func );
    std::cout << "---------------------------------------------------------\n";
}



// delete
void test_delete()
{
    std::cout << std::endl << "test DELETE" << std::endl;
    using namespace meta;

    auto constexpr s = create_struct( make_member<"one">( 1 ), make_member<"two">( 2UL ), make_member<"three">( 3.0f ) );
    constexpr auto print_func =  []<Member M>(M const& member) { std::cout << static_cast<std::string>(M::tag()) << ": " << member.value() << std::endl; return member.value(); };
    std::cout << "created structure:\n" << std::endl;
    map_struct( s, print_func );


    auto constexpr t = delete_struct<"one">( s );
    std::cout << "\nafter delete 'one', the structure becomes:" << std::endl;
    map_struct( t, print_func );

    auto constexpr u = delete_struct<"two">( t );
    std::cout << "\nafter delete 'two', the structure becomes:" << std::endl;
    map_struct( u, print_func );

    std::cout << "---------------------------------------------------------\n";
}

int main()
{
    test_create();
    test_read();
    test_update();
    test_delete();

    return 0;
}



