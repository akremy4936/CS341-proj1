#include <iostream>
#include "centcom.h"

bool testCentComConstructorNormal();
bool testCentComConstructorError();
bool testAddElevatorNormal();
bool testAddElevatorError();
bool testElevatorSetUpError();
bool testElevatorInsertFloorError();
bool testElevatorPushButtonNormal();
bool testElevatorPushButtonError();
bool testElevatorProcessNextRequestNormal();
bool testElevatorProcessNextRequestError();
bool testElevatorPushButtonDuplicates();
bool testElevatorSetUpBoundaries();
bool testElevatorDirectionReversal();
bool testElevatorHandlesMultipleRequests();
class Tester {
public:
    void runTests() {
        cout << "Test CentCom constructor (normal case): " << (testCentComConstructorNormal() ? "Passed" : "Failed") << endl;
        cout << "Test CentCom constructor (error case): " << (testCentComConstructorError() ? "Passed" : "Failed") << endl;
        cout << "Test CentCom::addElevator() (normal case): " << (testAddElevatorNormal() ? "Passed" : "Failed") << endl;
        cout << "Test CentCom::addElevator() (error case): " << (testAddElevatorError() ? "Passed" : "Failed") << endl;
        cout << "Test Elevator::setUp() (error case): " << (testElevatorSetUpError() ? "Passed" : "Failed") << endl;
        cout << "Test Elevator::insertFloor() (error case): " << (testElevatorInsertFloorError() ? "Passed" : "Failed") << endl;
        cout << "Test Elevator::pushButton() (normal scenario): " << (testElevatorPushButtonNormal() ? "Passed" : "Failed") << endl;
        cout << "Test Elevator::pushButton() (error scenario): " << (testElevatorPushButtonError() ? "Passed" : "Failed") << endl;
        cout << "Test Elevator::processNextRequest() (normal case): " << (testElevatorProcessNextRequestNormal() ? "Passed" : "Failed") << endl;
        cout << "Test Elevator::processNextRequest() (error case): " << (testElevatorProcessNextRequestError() ? "Passed" : "Failed") << endl;
        cout << "Test Elevator::pushButtonDuplicates() : " << (testElevatorPushButtonDuplicates() ? "Passed" : "Failed") << endl;
        cout << "Test Elevator::ElevatorSetUpBoundaries() : " << (testElevatorSetUpBoundaries() ? "Passed" : "Failed") << endl;
        cout << "Test Elevator::ElevatorDirectionReversal() : " << (testElevatorDirectionReversal() ? "Passed" : "Failed") << endl;
         cout << "Test Elevator::ElevatorHandlesMultipleRequests() : " << (testElevatorHandlesMultipleRequests() ? "Passed" : "Failed") << endl;
    }
};

int main() {
    Tester tester;
    tester.runTests();
    return 0;
}


// Test CentCom constructor (normal case)                                                                                                                                         
bool testCentComConstructorNormal() {
    CentCom centCom(5, 1001);  // 5 elevators, building ID 1001                                                                                                                   
    return (centCom.getElevator(0) == nullptr);  // No elevators are set initially                                                                                                
}

// Test CentCom constructor (error case)                                                                                                                                          
bool testCentComConstructorError() {
    CentCom centCom(0, 1001);  // 0 elevators                                                                                                                                     
    return (centCom.getElevator(0) == nullptr);  // No elevators should be available                                                                                              
}

// Test CentCom::addElevator() (normal case)                                                                                                                                      
bool testAddElevatorNormal() {
    CentCom centCom(5, 1001);
    return centCom.addElevator(0, 1, 10);  // Adding an elevator with ID 0, floors 1-10                                                                                           
}

// Test CentCom::addElevator() (error case)                                                                                                                                       
bool testAddElevatorError() {
    CentCom centCom(5, 1001);
    return !centCom.addElevator(5, 1, 10);  // Invalid elevator ID (should be between 0 and 4)                                                                                    
}
// Test Elevator::setUp() (error case)                                                                                                                                            
bool testElevatorSetUpError() {
    Elevator elev(0);

    // Set up the elevator with floors 1 to 5                                                                                                                                     
    elev.setUp(1, 5);

    // Try to insert an out-of-range floor (should fail)                                                                                                                          
    if (elev.insertFloor(6)) {
        return false;  // Should not insert floor 6                                                                                                                               
    }

    // Try inserting a floor below the minimum range (should also fail)                                                                                                           
    if (elev.insertFloor(-1)) {
        return false;  // Should not insert floor -1                                                                                                                              
    }

    return true;  // Test passed                                                                                                                                                  
}

bool testElevatorInsertFloorError() {
    Elevator elev(3);  // Start with the lowest floor at 3                                                                                                                        

    // Set up the elevator with floors (valid range 3 to 10)                                                                                                                      
    elev.setUp(3, 10);

    // inserting a duplicate floor (should fail)                                                                                                                                  
    if (elev.insertFloor(5)) {
        return false;
    }

    // Insert one below the lowest floor (should pass)                                                                                                                            
    if (!elev.insertFloor(2)) {
        return false;
    }

    // Insert another floor at the head (should pass)                                                                                                                             
    if (!elev.insertFloor(1)) {
        return false;
    }

    // Try inserting another duplicate (should fail)                                                                                                                              
    if (elev.insertFloor(1)) {
        return false;
    }

    // Try inserting a floor above the highest (should fail)                                                                                                                      
    if (elev.insertFloor(11)) {
        return false;
    }

    // Insert an underground floor (should pass)                                                                                                                                  
    if (!elev.insertFloor(0)) {
        return false;
    }

    // Try inserting another underground floor at the head (should pass)                                                                                                          
    if (!elev.insertFloor(-1)) {
        return false;
    }

    return true;
}

// Test Elevator::pushButton()                                                                                                                                                    
bool testElevatorPushButtonNormal() {
    // Create an elevator object with ID 0                                                                                                                                        
    Elevator elev(1);

    // Manually set up the floors                                                                                                                                                 
    elev.setUp(1, 5);

    // Request a valid floor within the allowed range (floor 3)                                                                                                                   
    bool result = elev.pushButton(3);  // Push button to go to floor 3                                                                                                            

    // Check the result and output the relevant debug information                                                                                                                 
    if (!result) {
        return false;  // Test failed if it couldn't handle the request                                                                                                           
    }

    return true;  // Test passed if no issues                                                                                                                                     
}


bool testElevatorPushButtonError() {
    // Create an elevator object with ID 0                                                                                                                                        
    Elevator elevator(0);

    // Manually set up the floors (e.g., floors 1 to 10)                                                                                                                          
    elevator.setUp(1, 10);

    // Test: Request a floor below the allowed range (floor 0)                                                                                                                    
    bool resultBelow = elevator.pushButton(0);  // Floor 0 is outside the range (1 to 10)                                                                                         
    if (resultBelow) {
        return false; // Test failed                                                                                                                                              
    }

    // Test: Request a floor above the allowed range (floor 11)                                                                                                                   
    bool resultAbove = elevator.pushButton(11);  // Floor 11 is outside the range (1 to 10)                                                                                       
    if (resultAbove) {
        return false; // Test failed                                                                                                                                              
    }

    // Test: Request a valid floor within the allowed range (floor 5)                                                                                                             
    bool resultValid = elevator.pushButton(5);  // Floor 5 is within the range (1 to 10)                                                                                          
    if (!resultValid) {
        return false; // Test failed                                                                                                                                              
    }

    return true; // All tests passed                                                                                                                                              
}

bool testElevatorProcessNextRequestNormal() {
    Elevator elev(0);
    elev.setUp(1, 5);
    elev.pushButton(3);  // Set a request for floor 3                                                                                                                             
    return elev.processNextRequest();  // Process next request (should move to floor 3)                                                                                           
}



// Test Elevator::processNextRequest() (error scenario)                                                                                                                           
bool testElevatorProcessNextRequestError() {
    Elevator elev(0);
    elev.pushButton(6);  // Invalid floor                                                                                                                                         
    return !elev.processNextRequest();  // No valid request, should return false                                                                                                  
}
bool testElevatorPushButtonDuplicates() {
    Elevator elev(0);
    elev.setUp(1, 5);

    elev.pushButton(3);  // Request floor 3                                                                                                                                       
    elev.pushButton(3);  // Request floor 3 again (duplicate)                                                                                                                     

    elev.processNextRequest();  // Process the first request (should stop at 3)                                                                                                   

    return true;
}

bool testElevatorSetUpBoundaries() {
    Elevator elev(0);

    // Test setting up with a single floor range                                                                                                                                  
    elev.setUp(5, 5);  // Single floor range                                                                                                                                      

    elev.setUp(-10, -1);  // Large negative range                                                                                                                                 
    // After setting up the elevator for floors from -10 to -1, the elevator should start at -10                                                                                  

    return true;
}
bool testElevatorDirectionReversal() {
    Elevator elev(0);
    elev.setUp(1, 5);

    elev.pushButton(5);  // Request floor 5                                                                                                                                       
    elev.processNextRequest(); // Moves to floor 5                                                                                                                                

    elev.pushButton(2);  // Now request a lower floor                                                                                                                             
    elev.processNextRequest(); // Should move down to 2                                                                                                                           

    return elev.processNextRequest() == false; // No more requests left                                                                                                           
}
bool testElevatorHandlesMultipleRequests() {
    Elevator elev(0);
    elev.setUp(1, 5);
    elev.pushButton(2);
    elev.pushButton(4);
    elev.processNextRequest();  // Should move to 2 first                                                                                                                         
    return elev.processNextRequest();  // Should then move to 4                                                                                                                   
}

