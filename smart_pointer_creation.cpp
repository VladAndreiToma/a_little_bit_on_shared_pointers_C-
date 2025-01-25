#include <iostream>
using namespace std;
// the generic functionality of smart pointer
// they are in general good as they hold functionality
// they free the memory after object deletion
// work similarly to java's garbage collector or c# garbage collector
// declared using template so we cover any type of object
template<class T> class SmartPtr{
  // smart pointer of class T that can take whatever structure or data type can be TBrowser , TCanvas , THisto , TGraph , TMath , TPoly , TTree , TNtuple , TBranch , TLeaf from ROOT
  // int , float , long .... std::vector , other classes so on
  T* ptr; // actual pointer of type T generic
  public:
    explicit SmartPtr( T* p = NULL ){ ptr = p; }  // declaring a pointer initial value null then passing what we want
    ~SmartPtr(){ delete(ptr);  }  // destructor should destruct the pointer ---> memory freeing character
    // now overloading the dereference operator and the get method operator
    T& operator*(){return *ptr;}  // we get the reference of type t of the value we point to
    // we use anonim function for the operator* to do whats supposed to do
    T* operator->(){return ptr;} // here we do the same what we get with method operator is actual value so T*
    
};

int main(){
  SmartPtr<int> examplePtr( new int() ); // allocate some int resource in the memory and have acces via examplePtr
  *examplePtr = 20;  // we put 20 in the "container" inside the heap owned by examplePtr value 20
  std::cout << "Value that we stored with our smart pointer is: " << *examplePtr << '\n';
}


// this is actually how the smart pointer works 
// author: engr. Vlad Toma