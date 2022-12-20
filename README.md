# Meta-Structure CRUD in C++20

------

## Basic Usage:

### Create a meta-structure:

```cpp
using namespace meta;

auto constexpr s = create_struct(   make_member<"one">( 1 ),
                                    make_member<"two">( 2UL ),
                                    make_member<"three">( 3.0f ) );
```

This creates a meta-structure that has three fields.

### Read a field in the meta-structure:

```cpp
auto constexpr two = read_struct<"two">( s );
// or
auto constexpr two = s.read<"two">();
```

This reads out the value stored in field 'two', and returns the result.

### Update a field in the meta-structure:

```cpp
auto constexpr t = update_struct<"one">( s, 1.01 );
// or
auto constexpr t = s.update<"one">( s, 1.01 );
```

This updates the value stored in field 'one' to 1.01, and returns a meta-structure reflecting the change.

### [Bonus] Upgrade a field in the meta-structure:

```cpp
auto s = create_struct( make_member<"func">( [](){ std::cout << "func called with null parameter.\n"; } ) );
auto S = upgrade_struct<"func">( s, []( int ){ std::cout << "func called with integer parameter.\n"; } );
read_struct<"func>( S )();
read_struct<"func>( S )( 1 );
```
alternatively, you can
```cpp
auto S = s.upgrade<"func">(  []( int ){ std::cout << "func called with integer parameter.\n"; } );
```



This mimicks an anonymous class with overloaded method `func`:

```cpp
struct anony
{
    void func() { std::cout << "func called with null parameter.\n"; }
    void func(int) { std::cout << "func called with integer parameter.\n"; }
};
```



### Delete a field in the meta-structure:

```cpp
auto constexpr t = delete_struct<"one">( s );
// or
auto constexpr t = s.destroy<"one">();
```

This will delete the field 'one' in the meta-structure, and returns a meta-structure without this field.


### [Bonus] Concatenate mutliple meta-structures

```cpp
auto constexpr s1 = create_struct( ...... );
auto constexpr s2 = create_struct( ...... );
auto constexpr s3 = create_struct( ...... );

auto constexpr s12 = concatenate_struct( s1, s2 );
auto constexpr s123 = concatenate_struct( s1, s2, s3 );
```

This will concatenate mutiple meta-structures, and returns a meta-structure holding all the fields.

### [Bonus] Map operations on every field in a meta-structure

```cpp
auto constexpr s = create_struct( make_member<"one">( 1 ), make_member<"two">( 2UL ), make_member<"three">( 3.0f ) );
constexpr auto print_func =  []<Member M>(M const& member) { std::cout << static_cast<std::string>(M::tag()) << ": " << member.value() << std::endl; return member.value(); };
map_struct( s, print_func );
```

This will apply the `print_func` to each filed in the meta-structure.


### [Bonus] Check if a field exist in a meta-structure

```cpp
auto constexpr s = create_struct( make_member<"one">( 1 ), make_member<"two">( 2UL ), make_member<"three">( 3.0f ) );
constexpr bool has_one = struct_has<"one">( s );
constexpr bool has_0 = struct_has<"0">( s );
```

This interface is friendly designed for `constexpr if`.



## [Bonus] Polymorphism

With a base class such as

```cpp
auto constexpr base = create_struct
(
    make_member<"f">( []( int x ){ std::cout << "call base::f with " << x << std::endl; } ),
    make_member<"g">( []( int x ){ std::cout << "call base::g with " << x << std::endl; } ),
    make_member<"h">( []( int x ){ std::cout << "call base::h with " << x << std::endl; } )
);

read_struct<"f">( base )( 1 );
read_struct<"g">( base )( 2 );
read_struct<"h">( base )( 3 );
```

We can mimick the creation of a derived class this way:

```cpp
auto constexpr derived_0 = update_struct<"f">( base,      []( int x ){ std::cout << "derived::f with " << x << std::endl; } );
auto constexpr derived_1 = update_struct<"g">( derived_0, []( int x ){ std::cout << "derived::g with " << x << std::endl; } );
auto constexpr derived =   update_struct<"h">( derived_1, []( int x ){ std::cout << "derived::h with " << x << std::endl; } );

read_struct<"f">( derived )( 1 );
read_struct<"g">( derived )( 2 );
read_struct<"h">( derived )( 3 );
```




## Tips and Pitfalls:

The `constexpr` specifier is guaranteed for each meta-structure instance if and only if the value type's ctor is `constexpr`-aware.

That is to say, in case the class `C1`'s ctor is `constexpr`:

```cpp
struct C1 // default ctor is constexpr
{
    int val_;
};
```

Then the result from `CRUD` operations is `constexpr` guaranteed. For example:

```cpp
auto constexpr s1 = create_struct( make_member<"xx">(true) );
auto constexpr s2 = update_struct<"xx">( C1{42} ); // <-- OK
```

However, if a class `C2`'s ctor is not `constexpr`:

```cpp
struct C2
{
    C2( int val ) : val_{ val } {} // <- not a constexpr ctor
    int val_;
};
```

Then the result from `CRUD` cannot be specified `constexpr`. For example:

```cpp
auto constexpr s1 = create_struct( make_member<"xx">(true) );
auto constexpr s2 = update_struct<"xx">( C2{42} ); // <-- NOT OK
auto s3 = update_struct<"xx">( C2{42} ); // <-- this is OK
```

It should be be emphasised that, even though `std::string`'s ctor is supposed to be `constexpr` by the standard, but for now (4/2/2022) gcc and llvm have not yet implemented this feature.



## Test Environment

- Linux x86_64 5.17.2
- GCC 11.2.0

## License

AGPLv3


## Acknowledgement

- <https://github.com/google/cpp-from-the-sky-down>
- <https://www.youtube.com/watch?v=FXfrojjIo80>


