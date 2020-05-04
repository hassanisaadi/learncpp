# 3. Object-Oriented Programming
## 3.1. `struct` vs `class`
Use `class` if the class has an invariant; use struct if the data members 
can vary independently. An "invariant" is a rule that limits the values of 
member variables.
Example:
```cpp
struct Pair {  // the members can vary independently
    string name;
    int volume;
};
```
vs.
```cpp
class Date {
public:
    // validate that {yy, mm, dd} is a valid date and initialize
    Date(int yy, Month mm, char dd);
    // ...
private:
    int y;
    Month m;
    char d;    // day
};
```

## 3.2. Classes
As a general rule, member data subject to an invariant should be specified 
`private`, in order to enforce the invariant before updating the member's value.

## 3.3. Scope Resolution
The process of determining which identifier to use is called "scope resolution". 
The scope resolution operator is `::`. We can use this operator to specify which 
namespace or class to search in order to resolve an identifier.
```cpp
Person::move(); \\ Call the move the function that is a member of the Person class.
std::map m; \\ Initialize the map container from the C++ Standard Library.  
```

* Classes: This becomes particularly useful if we want to separate class declaration class definition.
* Namespaces: allow programmers to group logically related variables and functions together.

## 3.4. Initializer list
```cpp
Date::Date(int d, int m, int y) : year(y) {
  Day(d);
  Month(m);
}
```

In the above example, `year` is initialized using an initializer list, but `day` and 
`month` are initialized using assignment. Using initializer list ensures we don't 
use the variable without being initialized.

### 3.4.1. Why Initialization list?
1. First, the compiler can optimize initialization faster from an initialization list than from within the constructor.
2. If you have a `const` class attribute, you can only initialize it using an initialization list. Otherwise, you would violate the `const` keyword simply by initializing the member in the constructor!
3. The third reason is that attributes defined as references must use initialization lists. Look at the following example on how references should use the initializer lists:
```cpp
#include <assert.h>
#include <string>
​
struct Person {
public:
  // TODO: Add an initialization list
  Person(std::string const & n) : name(n) {}
  std::string const name;
};
​
// Test
int main() {
  Person alice("Alice");
  Person bob("Bob");
  assert(alice.name != bob.name);
}
```

## 3.5. Accessor Functions
Accessor functions are public member functions that allow users to access an 
object's data, albeit indirectly.

### 3.5.1. `const` Accessor Function
Accessors should only retrieve data. They should not change the data stored in the object.

The main role of the `const` specifier in accessor methods is to protect member data. 
When you specify a member function as `const`, the compiler will prohibit that function 
from changing any of the object's member data.

```cpp
int Year() const{return year_;}
```

## 3.6. Mutator Functions
A mutator ("setter") function can apply logic ("invariants") when updating member data.

## 3.7. Abstraction
Abstraction refers to the separation of a class's interface from the details of its 
implementation. The interface provides a way to interact with an object, while hiding 
the details and implementation of how the class works.

Abstraction is used to expose only relevant information to the user. By hiding 
implementation details, we give ourselves flexibility to modify how the program works. 
In this example, you'll practice abstracting implementation details from the user.

In this exercise, you'll update the `class Sphere` so that it becomes possible to 
change the radius of a sphere after it has been initialized. In order to do this, 
you'll move the two class invariants into private member functions.

```cpp
#include <cassert>
#include <cmath>
#include <stdexcept>

class Sphere {
 public:
  Sphere(int radius) : radius_(radius), volume_(pi_ * 4 / 3 * pow(radius_, 3)) {
    if (radius <= 0) throw std::invalid_argument("radius must be positive");
  }

  int Radius() const { return radius_; }
  int Volume() const { return volume_; }

  // TODO: mutator
  void Radius(int radius) {
    if (radius <= 0) throw std::invalid_argument("radius must be positive");
    radius_ = radius;
    volume_ = pi_ * 4 / 3 * pow(radius_, 3);
  }

 private:
  float const pi_{3.14159};
  int radius_;
  float volume_;
};

// Test
int main(void) {
  Sphere sphere(5);
  assert(sphere.Radius() == 5);
  assert(abs(sphere.Volume() - 523.6) < 1);

  sphere.Radius(3);
  assert(sphere.Radius() == 3);
  assert(abs(sphere.Volume() - 113.1) < 1);

  bool caught{false};
  try {
    sphere.Radius(-1);
  } catch (...) {
    caught = true;
  }
  assert(caught);
}
```

## 3.8. Static Members
Class members can be declared `static`, which means that the member belongs to the entire class, 
instead of to a specific instance of the class. More specifically, a `static` member is created 
only once and then shared by all instances (i.e. objects) of the class. That means that if the 
`static` member gets changed, either by a user of the class or within a member function of the 
class itself, then all members of the class will see that change the next time they access the 
`static` member.

### 3.8.1. Implementation
`static` members are **declared** within their class (often in a header file) but in most cases they must 
be **defined** within the global scope. That's because memory is allocated for static variables immediately 
when the program begins, at the same time any global variables are initialized. Here is an example:
```cpp
#include <cassert>

class Foo {
 public:
  static int count;
  Foo() { Foo::count += 1; }
};

int Foo::count{0};

int main() {
  Foo f{};
  assert(Foo::count == 1);
}
```

An exception to the global definition of `static` members is if such members can be marked as `constexpr`. 
In that case, the `static` member variable can be both declared and defined within the class definition:
```cpp
struct Kilometer {
  static constexpr int meters{1000};
};
```

### 3.8.2. Static Method
In addition to `static` member variables, C++ supports `static` member functions 
(or "methods"). Just like `static` member variables, `static` member functions are 
instance-independent: they belong to the class, not to any particular instance of the class.

One corollary to this is that we can method invoke a `static` member function without ever 
creating an instance of the class.

```cpp
#include <cassert>
#include <cmath>
#include <stdexcept>

class Sphere {
 public:
   static float Volume(int radius) {
       return pi_ * 4/3 * pow(radius,3);
   }

 private:
  static float constexpr pi_{3.14159};
};

// Test
int main(void) {
  assert(abs(Sphere::Volume(5) - 523.6) < 1);
}
```

## 3.9. Access Modifiers in C++
* **Public**: All the class members declared under public will be available to everyone. 
The data members and member functions declared public can be accessed by other classes too. 
The public members of a class can be accessed from anywhere in the program using the direct 
member access operator (.) with the object of that class.
* **Private**: The class members declared as private can be accessed only by the functions 
inside the class. They are not allowed to be accessed directly by any object or function outside 
the class. Only the member functions or the friend functions are allowed to access the private 
data members of a class.
* **Protected**: Protected access modifier is similar to that of private access modifiers, 
the difference is that the class member declared as Protected are inaccessible outside the 
class but they can be accessed by any subclass(derived class) of that class.

[Source](https://www.geeksforgeeks.org/access-modifiers-in-c/)

## 3.10. Inheritence
Here's an example of how we can implement inheritence in C++. We have a *parent* `Vehicle` class:
```cpp
class Vehicle {
public:
  int wheels = 0;
  string color = "blue";

  void Print() const
  {
    std::cout << "This " << color << " vehicle has " << wheels << " wheels!\n";
  }
};
```
And we can derive from this class the other classes with different members. The following classes are called *derived*
or *child* classes:
```cpp
class Car : public Vehicle {
public:
  bool sunroof = false;
};

class Bicycle : public Vehicle {
public:
  bool kickstand = true;
};
```

### 3.10.1. Inherited Access Specifiers
* **Public inheritance**: the public and protected members of the base class listed after the 
specifier keep their member access in the derived class

* **Protected inheritance**: the public and protected members of the base class listed after the 
specifier are protected members of the derived class

* **Private inheritance**: the public and protected members of the base class listed after the 
specifier are private members of the derived class

[Source](https://en.cppreference.com/w/cpp/language/access)

## 3.11. Composition
Composition is a closely related alternative to inheritance. Composition involves constructing 
("composing") classes from other classes, instead of inheriting traits from a parent class.

A common way to distinguish "composition" from "inheritance" is to think about what 
an object can do, rather than what it is. This is often expressed as **"has a"** versus **"is a"**.

From the standpoint of composition, a cat "has a" head and "has a" set of paws and "has a" tail.

From the standpoint of inheritance, a cat "is a" mammal.

There is no hard and fast rule about when to prefer composition over inheritance. 
In general, if a class needs only extend a small amount of functionality beyond what 
is already offered by another class, it makes sense to inherit from that other class. 
However, if a class needs to contain functionality from a variety of otherwise unrelated 
classes, it makes sense to compose the class from those other classes.

```cpp
// Example solution for Circle class
#include <iostream>
#include <cmath>
#include <assert.h>
// Define PI
#define PI 3.14159;


// Define LineSegment struct
struct LineSegment {
// Define protected attribute length
public:
    double length;
};

// Define Circle class
class Circle {
public:
    Circle(LineSegment& radius);
    double Area();

private:
    LineSegment& radius_;
};

// Declare Circle class
Circle::Circle(LineSegment& radius) : radius_(radius) {}

double Circle::Area() 
{
    return pow(Circle::radius_.length, 2) * PI;
}

// Test in main()
int main() 
{
    LineSegment radius {3};
    Circle circle(radius);
    assert(int(circle.Area()) == 28);
}
```

## 3.12. Friends
In C++, `friend` classes provide an alternative inheritance mechanism to derived classes. 
The main difference between classical inheritance and friend inheritance is that a `friend` 
class can access private members of the base class, which isn't the case for classical 
inheritance. In classical inheritance, a derived class can only access public and protected 
members of the base class.

```cpp
// Example solution for Rectangle and Square friend classes
#include <assert.h>

// Declare class Rectangle
class Rectangle;

// Define class Square as friend of Rectangle
class Square {
// Add public constructor to Square, initialize side
public:
    Square(int s) : side(s) {}

private:
    // Add friend class Rectangle
    friend class Rectangle;
    // Add private attribute side to Square
    int side;
};

// Define class Rectangle
class Rectangle {
// Add public functions to Rectangle: area() and convert()
public:
    Rectangle(const Square& a);
    int Area() const;

private:
    // Add private attributes width, height
    int width {0};
    int height {0};
};

 // Define a Rectangle constructor that takes a Square
Rectangle::Rectangle(const Square& a) : width(a.side), height(a.side)
{
}

// Define Area() to compute area of Rectangle
int Rectangle::Area() const
{
    return width * height;
}
// Update main() to pass the tests
int main()
{
    Square square(4);
    Rectangle rectangle(square);
    assert(rectangle.Area() == 16); 
}
```

## 3.13. Polymorphism
Polymorphism is means "assuming many forms".

In the context of object-oriented programming, polymorphism describes a paradigm 
in which a function may behave differently depending on how it is called. 
In particular, the function will perform differently based on its inputs.

Polymorphism can be achieved in two ways in C++: **overloading** and **overriding**.

### 3.13.1. Overloading
In C++, you can write two (or more) versions of a function with the same name. 
This is called "overloading". Overloading requires that we leave the function 
name the same, but we modify the function signature. For example, we might 
define the same function name with multiple different configurations of input arguments.
```cpp
#include <ctime>

class Date {
public:
    Date(int day, int month, int year) : day_(day), month_(month), year_(year) {}
    Date(int day, int month) : day_(day), month_(month)  // automatically sets the Date to the current year
    {
        time_t t = time(NULL);
        tm* timePtr = localtime(&t);
        year_ = timePtr->tm_year;
    }

private:
    int day_;
    int month_;
    int year_;
};
```
#### 3.13.1.1. Operator Overloading
```cpp
#include <assert.h>

// TODO: Define Point class
class Point {
public:
  // TODO: Define public constructor
  Point(int x = 0, int y = 0) : x(x), y(y) {}

  // TODO: Define + operator overload
  Point operator+(const Point& addend) {
    Point sum;
    sum.x = x + addend.x;
    sum.y = y + addend.y;
    return sum;
  }

  // TODO: Declare attributes x and y
  int x, y;
};

// Test in main()
int main() {
  Point p1(10, 5), p2(2, 4);
  Point p3 = p1 + p2; // An example call to "operator +";
  assert(p3.x == p1.x + p2.x);
  assert(p3.y == p1.y + p2.y);
}
```

## 3.14. Virtual Functions
Virtual functions are a polymorphic feature. These functions are declared (and possibly defined) in a 
base class, and can be overridden by derived classes.

This approach declares an interface at the base level, but delegates the implementation of 
the interface to the derived classes.

In this exercise, `class Shape` is the base class. Geometrical shapes possess both an area and 
a perimeter. `Area()` and `Perimeter()` should be virtual functions of the base class interface. 
Append `= 0` to each of these functions in order to declare them to be **"pure"** virtual functions.

A pure virtual function is a virtual function that the base class declares but does not define.

A pure virtual function has the side effect of making its class abstract. This means that the 
class cannot be instantiated. Instead, only classes that derive from the abstract class and 
override the pure virtual function can be instantiated.
```cpp
class Shape {
    public:
        Shape() {}
        virtual double Area() const = 0;
        virtual double Perimeter() const = 0;
};
```
Virtual functions can be defined by derived classes, but this is not required. 
However, if we mark the virtual function with `= 0` in the base class, then we are 
declaring the function to be a pure virtual function. This means that the base 
class does not define this function. A derived class must define this function, 
or else the derived class will be abstract.

An example
```cpp
// Example solution for Shape inheritance
#include <assert.h>
#include <cmath>

// TODO: Define pi
#define PI 3.14159

// TODO: Define the abstract class Shape
class Shape{
  // TODO: Define public virtual functions Area() and Perimeter()
  // TODO: Append the declarations with = 0 to specify pure virtual functions
    public:
      virtual float Area() const = 0;
      virtual float Perimeter() const = 0;
};

// TODO: Define Rectangle to inherit publicly from Shape
class Rectangle : public Shape{
  // TODO: Declare public constructor
    public:
      Rectangle(int w, int h) : width(w), height(h) {}
  // TODO: Override virtual base class functions Area() and Perimeter()
      float Area() const { return width * height; }
      float Perimeter() const { return 2 * (height + width); }
  // TODO: Declare private attributes width and height
    private:
      int width;
      int height;
};

// TODO: Define Circle to inherit from Shape
class Circle : public Shape{
  // TODO: Declare public constructor
    public: Circle(float r) : radius(r) {}
  // TODO: Override virtual base class functions Area() and Perimeter()
      float Area() const { return PI * pow(radius, 2); }
      float Perimeter() const { return 2 * PI * radius; }
  // TODO: Declare private member variable radius
    private:
      float radius;
};

// Test in main()
int main() {
  double epsilon = 0.1; // useful for floating point equality

  // Test circle
  Circle circle(12.31);
  assert(abs(circle.Perimeter() - 77.35) < epsilon);
  assert(abs(circle.Area() - 476.06) < epsilon);

  // Test rectangle
  Rectangle rectangle(10, 6);
  assert(rectangle.Perimeter() == 32);
  assert(rectangle.Area() == 60);
}
```

## 3.15. Polymorphism: Overriding
**"Overriding"** a function occurs when:

1. A base class declares a `virtual` function.
2. A derived class overrides that virtual function by defining its own implementation with 
an identical function signature (i.e. the same function name and argument types).
```cpp
class Animal {
public:
  virtual std::string Talk() const = 0;
};

class Cat {
public:
  std::string Talk() const { return std::string("Meow"); }
};
```
In this example, `Animal` exposes a virtual function: `Talk()`, but does not define it. 
Because `Animal::Talk()` is undefined, it is called a pure virtual function, as opposed 
to an ordinary (impure? 😉) virtual function.

Furthermore, because `Animal` contains a pure virtual function, the user cannot instantiate 
an object of type `Animal`. This makes `Animal` an **abstract class**.

`Cat`, however, inherits from `Animal` and overrides `Animal::Talk()` with `Cat::Talk()`, 
which is defined. Therefore, it is possible to instantiate an object of type `Cat`.

## 3.16. Function Hiding
A derived class hides a base class function, as opposed to overriding it, 
if the base class function is not specified to be virtual.
```cpp
class Cat { // Here, Cat does not derive from a base class
public:
  std::string Talk() const { return std::string("Meow"); }
};

class Lion : public Cat {
public:
  std::string Talk() const { return std::string("Roar"); }
};
```
In this example, `Cat` is the base class and `Lion` is the derived class. 
Both `Cat` and `Lion` have `Talk()` member functions.

When an object of type `Lion` calls `Talk()`, the object will run `Lion::Talk()`, not `Cat::Talk()`.

In this situation, `Lion::Talk()` is hiding `Cat::Talk()`. If `Cat::Talk()` were virtual, 
then `Lion::Talk()` would override `Cat::Talk()`, instead of hiding it. Overriding 
requires a virtual function in the base class.

The distinction between overriding and hiding is subtle and not terribly significant, but in 
certain situations hiding can lead to bizarre errors, particularly when the two functions 
have slightly different function signatures.

## 3.17. `override`
"Overriding" a function occurs when a derived class defines the implementation of a 
virtual function that it inherits from a base class.

It is possible, but not required, to specify a function declaration as override.
```cpp
class Shape {
public:
  virtual double Area() const = 0;
  virtual double Perimeter() const = 0;
};

class Circle : public Shape {
public:
  Circle(double radius) : radius_(radius) {}
  double Area() const override { return pow(radius_, 2) * PI; } // specified as an override function
  double Perimeter() const override { return 2 * radius_ * PI; } // specified as an override function

private:
  double radius_;
};
```
This specification tells both the compiler and the human programmer that the purpose of this 
function is to override a virtual function. The compiler will verify that a function specified 
as override does indeed override some other virtual function, or otherwise the compiler will 
generate an error.

Specifying a function as `override` is good practice, as it empowers the compiler to verify the code, 
and communicates the intention of the code to future users.
