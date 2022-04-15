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

    auto constexpr t = create_struct( make_member<"a">( 'a' ) );
    map_struct( t, print_func );

    auto constexpr u = create_struct( make_member<"abc">( "abc" ) );
    map_struct( u, print_func );

    auto w = create_struct( make_member<"def">( std::string{"def"} ) ); // <-- not constexprable
    map_struct( w, print_func );

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

struct id
{
    int id_;
};

std::ostream& operator<<(std::ostream& os, const id& id)
{
    return os << id.id_;
}

struct jd
{
    jd( int jd__ ) : jd_( jd__ ) {} // not constexpr
    int jd_;
};

std::ostream& operator<<(std::ostream& os, const jd& jd)
{
    return os << jd.jd_;
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
    constexpr auto u = update_struct<"two">( t, id{1270} );
    //constexpr auto u = update_struct<"two">( t, "12799" );
    map_struct( u, print_func );

    std::cout << "\nafter update three with 'c'(char), the structure becomes:" << std::endl;
    auto v = update_struct<"three">( u, jd{13} ); // <-- not constexpr
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

    auto constexpr w = delete_struct<"three">( u );
    std::cout << "\nafter delete 'three', the structure becomes:" << std::endl;
    map_struct( w, print_func );

    std::cout << "---------------------------------------------------------\n";
}


void test_polymorphism()
{
    std::cout << std::endl << "test polymorphism" << std::endl;
    using namespace meta;

    auto constexpr base = create_struct
    (
        make_member<"f">( []( int x ){ std::cout << "call base::f with " << x << std::endl; } ),
        make_member<"g">( []( int x ){ std::cout << "call base::g with " << x << std::endl; } ),
        make_member<"h">( []( int x ){ std::cout << "call base::h with " << x << std::endl; } )
    );

    read_struct<"f">( base )( 1 );
    read_struct<"g">( base )( 2 );
    read_struct<"h">( base )( 3 );

    auto constexpr derived_0 = update_struct<"f">( base,      []( int x ){ std::cout << "derived::f with " << x << std::endl; } );
    auto constexpr derived_1 = update_struct<"g">( derived_0, []( int x ){ std::cout << "derived::g with " << x << std::endl; } );
    auto constexpr derived =   update_struct<"h">( derived_1, []( int x ){ std::cout << "derived::h with " << x << std::endl; } );

    read_struct<"f">( derived )( 1 );
    read_struct<"g">( derived )( 2 );
    read_struct<"h">( derived )( 3 );
}

void test_has()
{
    std::cout << std::endl << "test struct_has" << std::endl;
    using namespace meta;

    auto constexpr s = create_struct( make_member<"one">( 1 ), make_member<"two">( 2UL ), make_member<"three">( 3.0f ) );
    constexpr auto print_func =  []<Member M>(M const& member) { std::cout << static_cast<std::string>(M::tag()) << ": " << member.value() << std::endl; return member.value(); };
    std::cout << "created structure:\n" << std::endl;
    map_struct( s, print_func );

    {
        constexpr bool has_one = struct_has<"one">( s );
        if ( has_one )
            std::cout << "struct has field ONE.\n";
        else
            std::cout << "struct does not have field ONE.\n";
    }


    auto constexpr t = delete_struct<"one">( s );
    std::cout << "\nafter delete 'one', the structure becomes:" << std::endl;
    map_struct( t, print_func );
    {
        constexpr bool has_one = struct_has<"one">( t );
        if ( has_one )
            std::cout << "struct has field ONE.\n";
        else
            std::cout << "struct does not have field ONE.\n";
    }


    std::cout << "---------------------------------------------------------\n";
}



int main()
{
    test_create();
    test_read();
    test_update();
    test_delete();
    test_has();

    test_polymorphism();

    return 0;
}




