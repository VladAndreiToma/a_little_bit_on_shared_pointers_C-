#include <iostream>
using namespace std;
#include <memory>
class myDataCollection{
  public:
    void response(){  std::cout << "Hello from DataCollection..." << std::endl; }
};

// lets try with some other class that is more advanced like Car
class Car{
  private:
    std::string name;
  public:
    Car(std::string carName) : name(std::move(carName)) {
    std::cout << "Car created: " << name << '\n';
}
    void printCarName(){
      std::cout << "Car the car name is: " << name << '\n';
    }
    ~Car(){
      std::cout << "Destroying car with name: " << name << '\n';
    }
};

// shared pointer makes it possible to have copy of object handlers, i.e pointers to a class or other object
// thus we share the domain of that object and we can assign new pointer or keep track of how many access points we have at that address

int main() 
{
    std::shared_ptr<myDataCollection>  shrdP1( new myDataCollection );
    std::cout << shrdP1 . get() << '\n';
    shrdP1 -> response();
    
    // now sharing ownership
    std::shared_ptr<myDataCollection> p2( shrdP1 );
    
    // another one
    std::shared_ptr<myDataCollection> p3( p2 );
    
    // lets see what happens
    std::cout << "from shared pointer 2: " << p2 . get() << '\n';
    std::cout << "from shared pointer 3: " << p3 . get() << '\n'; 
    // so what we ve seen is that the object share the same memory address
    p2->response();
    p3->response();
    // the same method is available since is the same object
    
    // how many instances we have that share the same object
    std::cout << "# of shares: " << p3 . use_count() << '\n';
    
    // looking at uniqueness
    std::cout << "unique character seen from p1 " << shrdP1 . unique() << '\n';
    //std::cout << "unique character seen from p2 " << p2 . unique() << '\n';
    // no matter how we put the things we created shared resource thus result is 0 
    
    // but we can create another object
    std::shared_ptr fP1 = make_shared<myDataCollection>();
    std::cout << "is fP1 unique? " << fP1 . unique() << " 1 corresponds to true" <<'\n';
    // now if i add a resource then i ll have 0 for both cause its not unique holding anymore 
    

    // lets see what happens with class car
    std::shared_ptr carP1 = make_shared<Car>("Honda");
    carP1 -> printCarName();
    std::shared_ptr<Car> carP2( carP1 );
    carP2 -> printCarName();
}
// so in car we see that even tho we have constructors the behavior is the same
// we can easily create an manipulate the same object without new pointers but rather with copies or shared ptrs