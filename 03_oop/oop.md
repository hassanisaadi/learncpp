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

## 3.9. Inheritence
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

### 3.9.1. Inherited Access Specifiers
* **Public inheritance**: the public and protected members of the base class listed after the specifier keep their member access in the derived class

* **Protected inheritance**: the public and protected members of the base class listed after the specifier are protected members of the derived class

* **Private inheritance**: the public and protected members of the base class listed after the specifier are private members of the derived class

