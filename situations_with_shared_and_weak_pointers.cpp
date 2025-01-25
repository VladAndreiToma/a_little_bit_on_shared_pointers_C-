// most common ground we can encouter shared pointers in in memory control and access
// thus lets try some examples


// 1st one might refer to databases
// making a class handling some database
#include <iostream>
#include <memory>
#include <string>
class TestDatabase{
  public:
    TestDatabase(const std::string& dbName) : name(dbName){
      std::cout << "connect to database: " << name << '\n';
    }
    ~TestDatabase(){
      std::cout << "disconnect from database: " << name << '\n';
    }
    void query(const std::string& sqlInstruction){
      std::cout << "executing query sqlInstruction: " << sqlInstruction << '\n';
    }
  private:
    std::string name; // available to classes inheriting from this one
};
/*
int main(){
  std::shared_ptr<TestDatabase> DB = std::make_shared<TestDatabase>("ATestDatabase");
  {
    // making use of scoping in inner degrees
    // made an initial ownership object now next instances of ownership
    std::shared_ptr<TestDatabase> user1 = DB; // or ....user2(db)
    user1 -> query( "Select * From Chebysev polynomials roots" );
    {
      std::shared_ptr<TestDatabase> user2 = DB;
      user2 -> query("Insert into Chebysev polynomials roots Values('0.03*pi*log(20)' , 'e^0.5+sqrt(3)')");
    }
  }
  // user1 executes and holds scope
  // so user2 executes and drops scopes - db still exists
  // then user1 dropes scope - db still exists
  // db destroyed at block of main execution stopped
  
  // here shared pointer is useful cause we hold access to same resource i.e. the database when we need in ordered contexts to perform some sort of operations in very specific orders

  return 0;
}*/



// 2. strcutrues like graphs or trees
// nodes have ownership of other nodes its good for flow control and prevent memory leaks
#include <vector> // the rest we have

class Node{
  public:
    int value;
    std::vector<std::shared_ptr<Node>> children;
    Node(int val) : value( val ){
      std::cout << "Node " << value << " created!" << '\n';
    }
    ~Node(){
      std::cout << "destroying node: " << value << '\n';
    }
    void addChild(const std::shared_ptr<Node>& child){
      children.push_back( child );
    }
};

/*int main(){
  std::shared_ptr<Node> root = std::make_shared<Node>(1); // making the root of the tree
  std::shared_ptr<Node> child1 = std::make_shared<Node>(2);
  std::shared_ptr<Node> child2 = std::make_shared<Node>(3);
  
  root -> addChild( child1 );
  root -> addChild( child2 ); // so here we share ownership of childs with childs themselves as any of them allow for such accessing
  
  std::cout << "Tree structure created\n";
  // all of objects have to go out of scope so all nodes are destroyed
  
  return 0;
}
*/

// 3. a cyclicity break , parent child
// having the needed libraries

struct Child;

struct Parent{
  std::shared_ptr<Child> child;
  ~Parent(){
    std::cout << "Parent destroyed \n";
  }
};

struct Child{
  std::weak_ptr<Parent> parent;
  ~Child(){std::cout<<"Child destroyed \n";}
};
// making use of another smart pointer that is std::weak_ptr its non owning pointer so breaks the cilclity of pointing endlessly :)

int main(){
  {
    // inner contexting
    auto parent = std::make_shared<Parent>();
    auto child = std::make_shared<Child>(); // initialize 2 shared pointers --- they hold responsibility for the object lifetime
    parent -> child = child; // as child allows for shared control and access it is assign to parent
    child -> parent = parent; // the same for parent
  }
}

