# Meta Structure CRUD in C++20

------

## Basic Usage:

### Create a meta structure:

```cpp
using namespace meta;

auto constexpr s = create_struct(   make_member<"one">( 1 ),
                                    make_member<"two">( 2UL ),
                                    make_member<"three">( 3.0f ) );
```

This creates a meta-structure that has three fields.

### Read a field in the meta structure:

```cpp
auto constexpr two = read_struct<"two">( s );
```

This reads out the value stored in field 'two', and returns this value.

### Update a field in the meta structure:

```cpp
auto constexpr t = update_struct<"one">( s, 1.01 );
```

This updates the value stored in field 'one' to 1.01, and returns a meta-structure reflecting the update.


### Delete a field in the meta structure:

```cpp
auto constexpr t = delete_struct<"one">( s );
```

This will delete the field 'one' in the meta-structure, and returns a meta-structure without this field.


## License

AGPLv3



