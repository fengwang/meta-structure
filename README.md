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
```

This reads out the value stored in field 'two', and returns the result.

### Update a field in the meta-structure:

```cpp
auto constexpr t = update_struct<"one">( s, 1.01 );
```

This updates the value stored in field 'one' to 1.01, and returns a meta-structure reflecting the change.


### Delete a field in the meta-structure:

```cpp
auto constexpr t = delete_struct<"one">( s );
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



## License

AGPLv3



