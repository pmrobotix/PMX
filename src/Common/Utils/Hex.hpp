#ifndef COMMON_HEX_HPP_
#define COMMON_HEX_HPP_

#include <iostream>

template<typename T>
inline T highbit(T& t)
{
	return t = (((T) (-1)) >> 1) + 1;
}

/*!
 *  Display binary value with 8 digits directly in C++
 *
 *  #include <bitset>
 *
 *  cout << std::bitset<8>(unsigned long number) << endl;
 */

/*!
 *  Display hexadecimal value with 4 digits directly in C++
 *
 *  #include <iostream>
 *  #include  <iomanip>
 *
 *  cout << setfill('0') << setw(4) << std::hex << hexa << endl;
 *  or
 *  cout << setfill('-') << setw(4) << std::hex << hexa << endl;
 */

/*!
 *  Display binary value
 * @param value
 * @param o (= std::cout)
 * @return
 */
/* example:
 cout << "hexPosition=" << hex << hexPosition << " dec=" << dec << hexPosition << " bin=";
 bin(hexPosition, cout);
 cout << endl;
 */
template<typename T>
std::ostream& bin(T& value, std::ostream &o)
{
	for (T bit = highbit(bit); bit; bit >>= 1)
	{
		o << ((value & bit) ? '1' : '0');
	}
	return o;
}

#endif
