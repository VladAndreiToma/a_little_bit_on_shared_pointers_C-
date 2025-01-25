// lets assume the next scenario in daq and analysis with ROOT
// lets say we want to write data from the acquisition pipelines
// which are threaded , so they run parallel and concurrent in the access points
// we want to store data visually in a nice form that might be interpreted with ease in Graphs or Histograms objects
// for that we use the concept of sharing ownership of the resources, the graph or hist objects one copy from each thread with smart copy mechanisms and memory usage
// for that we use shared pointers
// the belong to std::shared_ptr  ----> make_shared
// for this course we will build a template
// author: engr. Vlad Toma

#include <iostream>
#include <atomic>
#include <mutex>
#include <thread>
#include <TGraph.h>

std::mutex mutex; // Global mutex for thread-safe TGraph access

// SharedPointer Template
template<class T>
class SharedPointer {
    T* ptr;
    std::atomic<int>* refCount;

public:
    explicit SharedPointer(T* p = nullptr) 
        : ptr(p), refCount(new std::atomic<int>(p ? 1 : 0)) {}

    SharedPointer(const SharedPointer& other) {
        ptr = other.ptr;
        refCount = other.refCount;
        if (ptr) {
            (*refCount)++;
        }
    }

    SharedPointer(SharedPointer&& other) noexcept {
        ptr = other.ptr;
        refCount = other.refCount;
        other.ptr = nullptr;
        other.refCount = nullptr;
    }

    ~SharedPointer() {
        release();
    }

    SharedPointer& operator=(const SharedPointer& other) {
        if (this != &other) {
            release();
            ptr = other.ptr;
            refCount = other.refCount;
            if (ptr) {
                (*refCount)++;
            }
        }
        return *this;
    }

    SharedPointer& operator=(SharedPointer&& other) noexcept {
        if (this != &other) {
            release();
            ptr = other.ptr;
            refCount = other.refCount;
            other.ptr = nullptr;
            other.refCount = nullptr;
        }
        return *this;
    }

    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }
    T* get() const { return ptr; }

    void release() {
        if (refCount && --(*refCount) == 0) {
            delete ptr;
            delete refCount;
        }
    }

    int use_count() const { return refCount ? *refCount : 0; }
};

// Worker thread function
void worker(SharedPointer<TGraph>& sharedGraphAccessPoint, int threadId, int info1, int info2) {
    std::lock_guard<std::mutex> lock(mutex); // Protect access to TGraph
    sharedGraphAccessPoint->SetPoint(threadId, info1, info2);
}

// Plot the graph points
void PlotTheGraphPoints(SharedPointer<TGraph> theSharedGraph) {
    for (int i = 0; i < theSharedGraph->GetN(); ++i) {
        std::cout << "Point " << i << " --- coordinates: "
                  << theSharedGraph->GetX()[i] << " / "
                  << theSharedGraph->GetY()[i] << "\n";
    }
}

int main() {
    SharedPointer<TGraph> graph(new TGraph());
    int infoThread1[] = {100, 2000};
    int infoThread2[] = {10, 30000};
    int infoThread3[] = {500, 5000};

    // Create worker threads
    std::thread workingThread1(worker, std::ref(graph), 1, infoThread1[0], infoThread1[1]);
    std::thread workingThread2(worker, std::ref(graph), 2, infoThread2[0], infoThread2[1]);
    std::thread workingThread3(worker, std::ref(graph), 3, infoThread3[0], infoThread3[1]);

    workingThread1.join();
    workingThread2.join();
    workingThread3.join();

    PlotTheGraphPoints(graph);

    return 0;
}
