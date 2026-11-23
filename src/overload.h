#ifndef OVERLOAD_H
#define OVERLOAD_H

#endif//OVERLOAD_H
#ifdef OVERLOAD_IMPLEMETATION
#define overload_arguments(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_mac,...) _mac
#define overload_functions(_prefix,...) \
  overload_arguments(__VA_ARGS__,\
    _prefix##9,_prefix##8,_prefix##7,_prefix##6,_prefix##5,\
    _prefix##4,_prefix##3,_prefix##2,_prefix##1)(__VA_ARGS__)
#define template(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10) 
#endif
//a b c d e f g h i j k l m n o p q r s t u v w x y z
//A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
//1 2 3 4 5 6 7 8 9 0
