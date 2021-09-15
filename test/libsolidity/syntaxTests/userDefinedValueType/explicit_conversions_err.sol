type MyUInt is uint;
type MyAddress is address;
type AnotherUInt is uint;

function f() pure {
    MyUInt(-1);
    MyAddress(-1);
    MyUInt(5);
    MyAddress(address(5));

    AnotherUInt(MyUInt.wrap(5));
    MyUInt(AnotherUInt.wrap(10));
    AnotherUInt.unwrap(MyUInt.wrap(5));
    MyUInt.unwrap(AnotherUInt.wrap(10));
}
// ----
// TypeError 9640: (99-109): Explicit type conversion not allowed from "int_const -1" to "userdefined MyUInt".
// TypeError 9640: (115-128): Explicit type conversion not allowed from "int_const -1" to "userdefined MyAddress".
// TypeError 9640: (134-143): Explicit type conversion not allowed from "int_const 5" to "userdefined MyUInt".
// TypeError 9640: (149-170): Explicit type conversion not allowed from "address" to "userdefined MyAddress".
// TypeError 9640: (177-204): Explicit type conversion not allowed from "userdefined MyUInt" to "userdefined AnotherUInt".
// TypeError 9640: (210-238): Explicit type conversion not allowed from "userdefined AnotherUInt" to "userdefined MyUInt".
// TypeError 9553: (263-277): Invalid type for argument in function call. Invalid implicit conversion from userdefined MyUInt to userdefined AnotherUInt requested.
// TypeError 9553: (298-318): Invalid type for argument in function call. Invalid implicit conversion from userdefined AnotherUInt to userdefined MyUInt requested.
