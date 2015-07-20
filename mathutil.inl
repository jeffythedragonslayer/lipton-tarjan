template<typename T> T minimize (T& a, T b) {if( b < a ) a = b; return a;}
template<typename T> T maximize (T& a, T b) {if( b > a ) a = b; return a;}
template<typename T> int sgn(T v) {return (T(0) < v) - (v < T(0));}