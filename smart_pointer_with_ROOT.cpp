// lets assume we installed ROOT( cern's data analysis and scientific computing tool )
// we have the classes available , we have the dependencies
// lets include some classes

#include <iostream>
#include <TGraph.h>
#include <TTree.h>
#include <TNtuple.h>
#include <fstream>

using namespace std;
ofstream fout( "datapoints.log" );

// generic structure definition of a smart pointer again...
template<class T>
class CustomMadeSmartPointer{
    // strictly belongs to class
    T* ptr; // pointer instance of generic type T , to be established at declaration
    public:
        explicit CustomMadeSmartPointer(T* p = NULL){ ptr = p; }
    ~CustomMadeSmartPointer(){ delete ptr; } // destructor
    T& operator*(){ return *ptr; }  // overloading deref op
    T* operator->(){ return ptr; } // method accessing op overloading
};

int main(){
    // lets play with root libraries like TGraph
    CustomMadeSmartPointer<TGraph> ptrToGraphWithData( new TGraph() );
    // we do something here with the graph then the memory is dropped automatically
    ptrToGraphWithData -> SetPoint( 0 , 100.0 , 1000.0 ); // just eg. purpose
    ptrToGraphWithData -> SetLineColor( "kRED" );
    ptrToGraphWithData -> SetFillOpacity( "kTRUE" ); // string literal interpreted after as hexa color or binary for yes/ no req
    ptrToGraphWithData -> SetFillColor( "rgb(220.,220.,220., 0.8)" );
    // so we put some points make the graphic accept fill translucent and some line color
    ptrToGraphWithData -> SetLineStyle( 1 ); // normal line
    //get access to stuff
    fout << ptrToGraphWithData -> GetX()[0] << std::endl; // get some points from axis lets say   

    // usage with TTree
    CustomMadeSmartPointer<TTree> treePtr( new TTree( "tree" , "A Simple Tree" ) );
    int value = 42;
    treePtr -> Branch("value" , &value , "value/I"); // branch of data
    treePtr -> fill(); // put some data inside
    std::cout << "Tree has:  " << treePtr->GetEntries() << " entries.\n";

    return 0;
}