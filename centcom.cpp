#include "centcom.h"
CentCom::CentCom(int numElevators,int buildingID){
  m_id = buildingID;
  m_numElevators = numElevators;
  m_elevatorsList = new Elevator*[m_numElevators];
  for (int i = 0; i < m_numElevators; i++) {
    m_elevatorsList[i] = nullptr;
  }
}
CentCom::~CentCom(){
  for (int i = 0; i < m_numElevators; i++) {
    delete m_elevatorsList[i];
  }
  delete[] m_elevatorsList;
}
bool CentCom::addElevator(int ID, int bottomFloor, int topFloor){
  if (ID < 0 || ID >= m_numElevators || m_elevatorsList[ID] != nullptr) {
    return false; // Invalid ID or elevator already exists                                                                                                                        
  }
  m_elevatorsList[ID] = new Elevator(ID);
  m_elevatorsList[ID]->setUp(bottomFloor, topFloor); // Initialize floors                                                                                                         
  return true;
}
Elevator* CentCom::getElevator(int ID){
  if (ID < 0 || ID >= m_numElevators) {
    return nullptr; // Out-of-bounds check                                                                                                                                        
  }
  return m_elevatorsList[ID];
}
bool CentCom::setSecure(int ID, int floorNum, bool yes_no){
  Elevator* elev = getElevator(ID);
  if (elev == nullptr) return false;
  // Traverse floors and set security status                                                                                                                                      
  Floor* temp = elev->m_bottom; // Start from bottom to find the floor                                                                                                            
  while (temp) {
    if (temp->m_floorNum == floorNum) {
      temp->m_secured = yes_no; // Correctly update security                                                                                                                      
      return true;
    }
    temp = temp->m_next;
  }
  return false; // Floor not found                                                                                                                                                
}
bool CentCom::clearEmergency(int ID){
  Elevator* e = getElevator(ID);
  if (e == nullptr) return false;
  e->pushEmergency(false);
  return true;
}
Elevator::Elevator(int ID){
  m_id = ID;
  m_bottom = nullptr;
  m_top = nullptr;
  m_currentFloor = nullptr;
  m_upRequests = nullptr;
  m_downRequests = nullptr;
  m_moveState = IDLE;
  m_doorState = CLOSED;
  m_emergency = false;
  m_load = 0;
  m_isIdle=true;
}
Elevator::~Elevator(){
  clear(); // Clears all allocated floors and requests                                                                                                                            
}
void Elevator::clear(){
  // Clear m_bottom linked list                                                                                                                                                   
  while (m_bottom) {
    Floor* temp = m_bottom;
    m_bottom = m_bottom->m_next;
    delete temp;
  }
  // Clear m_upRequests and m_downRequests                                                                                                                                        
  while (m_upRequests) {
    Floor* temp = m_upRequests;
    m_upRequests = m_upRequests->m_next;
    delete temp;
  }
  while (m_downRequests) {
    Floor* temp = m_downRequests;
    m_downRequests = m_downRequests->m_next;
    delete temp;
  }
  m_top = nullptr;
  m_currentFloor = nullptr;
  m_load = 0;
  m_emergency = false;
  m_moveState = IDLE;
  m_doorState = CLOSED;
}

void Elevator::setUp(int firstFloor, int lastFloor) {
    // Ensure current floor is initialized to the firstFloor                                                                                                                      
    m_bottom = new Floor(firstFloor);
    m_top = new Floor(lastFloor);
    m_currentFloor = m_bottom;  // Set m_currentFloor to the bottom floor (first floor)                                                                                           

    // Create the floors in the range from firstFloor to lastFloor                                                                                                                
    Floor* current = m_bottom;
    for (int i = firstFloor + 1; i <= lastFloor; ++i) {
        Floor* newFloor = new Floor(i);
        // Insert the new floor at the beginning of the list                                                                                                                      
        current->m_next = newFloor;
        newFloor->m_previous = current;
        current = newFloor;
    }

    // Setting the top floor                                                                                                                                                      
    m_top = current;
    m_isIdle=true;
}
bool Elevator::insertFloor(int floor) {
    // Check if the floor already exists                                                                                                                                          
    Floor* current = m_bottom;
    while (current) {
        if (current->m_floorNum == floor) {
            cout << "Floor " << floor << " already exists. Cannot insert duplicate." << endl;
            return false;  // No duplicates allowed                                                                                                                               
        }
        current = current->m_next;
    }

    // If the list is empty, insert the first floor                                                                                                                               
    if (m_bottom == nullptr) {
        m_bottom = m_top = new Floor(floor);
        cout << "Inserted first floor: " << floor << endl;
        cout << "m_bottom updated to: " << m_bottom->m_floorNum << endl;
        return true;
    }

    // Insert the new floor just below the current m_bottom (i.e., m_bottom - 1)                                                                                                  
    if (floor == m_bottom->m_floorNum - 1) {
        // Create a new floor and insert it at the head (just below m_bottom)                                                                                                     
        Floor* newFloor = new Floor(floor);
        newFloor->m_next = m_bottom;
        m_bottom->m_previous = newFloor;
        m_bottom = newFloor;  // Update m_bottom to the new floor                                                                                                                 

        cout << "Inserted new bottom floor: " << floor << endl;
        cout << "m_bottom updated to: " << m_bottom->m_floorNum << endl;
    } else {
        cout << "Floor " << floor << " is out of bounds or not one below the current bottom ("
                  << m_bottom->m_floorNum << "). Cannot insert." << endl;
        return false; // Only allow insertion if it's one below the current lowest floor                                                                                          
    }

    return true;
}


bool Elevator::checkSecure(int floor){
  // Traverse through the floor list to find the floor                                                                                                                            
  Floor* temp = m_top;
  while (temp != nullptr) {
    if (temp->m_floorNum == floor) {
      // If the floor is found, check if it is secured                                                                                                                            
      return temp->m_secured; // Assuming m_secured exists in Floor class                                                                                                         
    }
    temp = temp->m_previous;
  }
  // Floor not found, return false as an error case                                                                                                                               
  return false;
}
void Elevator::pushEmergency(bool pushed){
  if (pushed) m_emergency = true;
}

bool Elevator::pushButton(int floor) {
    if (m_currentFloor == nullptr) {
        cout << "Error: Current floor is nullptr." << endl;
        return false;  // If currentFloor is null, return false                                                                                                                   
    }


    // Check if the floor is within the valid range (between m_bottom and m_top)                                                                                                  
    if (floor < m_bottom->m_floorNum || floor > m_top->m_floorNum) {
        cout << "Floor " << floor << " is out of range." << endl;
        return false; // Return false if the requested floor is out of range                                                                                                      
    }
    m_isIdle=false;
    Floor* request = new Floor(floor);

    // Debug: Show the floor we are going to insert and its direction                                                                                                             
    if (floor > m_currentFloor->m_floorNum) {
        cout << "Requesting upward to floor " << floor << endl;

        // Insert into upRequests in sorted order                                                                                                                                 
        if (!m_upRequests || m_upRequests->m_floorNum > floor) {
            cout << "Inserting at the head of the upRequests list." << endl;
            request->m_next = m_upRequests;
            m_upRequests = request;
        } else {
            Floor* temp = m_upRequests;
            while (temp->m_next && temp->m_next->m_floorNum < floor) {
                temp = temp->m_next;
            }
            cout << "Inserting into the middle or end of the upRequests list." << endl;
            request->m_next = temp->m_next;
            temp->m_next = request;
        }
    } else {
        cout << "Requesting downward to floor " << floor << endl;

        // Insert into downRequests in sorted order                                                                                                                               
        if (!m_downRequests || m_downRequests->m_floorNum < floor) {
            cout << "Inserting at the head of the downRequests list." << endl;
            request->m_next = m_downRequests;
            m_downRequests = request;
        } else {
            Floor* temp = m_downRequests;
            while (temp->m_next && temp->m_next->m_floorNum > floor) {
                temp = temp->m_next;
            }
            cout << "Inserting into the middle or end of the downRequests list." << endl;
            request->m_next = temp->m_next;
            temp->m_next = request;
        }
    }

    cout << "Request for floor " << floor << " successfully added." << endl;
    return true; // Return true when the request is successfully added                                                                                                            
}


bool Elevator::processNextRequest() {
    // Check for emergency situation, don't move in case of emergency                                                                                                             
    if (m_emergency) return false;

    // First, check if we have any requests in the up direction                                                                                                                   
    if (m_upRequests) {
        // Sort the upRequests list in ascending order                                                                                                                            
        sortRequestsAscending();

        // Now, process the next floor in the upRequests list                                                                                                                     
        Floor* temp = m_upRequests;
        while (temp) {
            // Check if the floor is secured using checkSecure method                                                                                                             
            if (checkSecure(temp->m_floorNum)) {
                // Skip this secured floor                                                                                                                                        
                temp = temp->m_next;
                continue; // Skip to the next floor                                                                                                                               
            }

            // If the floor is not secured, process it                                                                                                                            
            m_currentFloor = temp;
            m_upRequests = temp->m_next;  // Remove the processed floor from the list                                                                                             
            m_moveState = UP;  // Elevator is moving up                                                                                                                           
            m_isIdle=false;
            return true;
        }
    }

    // Check if we have any requests in the down direction                                                                                                                        
    if (m_downRequests) {
        // Sort the downRequests list in descending order                                                                                                                         
        sortRequestsDescending();

        // Now, process the next floor in the downRequests list                                                                                                                   
        Floor* temp = m_downRequests;
        while (temp) {
            // Check if the floor is secured using checkSecure method                                                                                                             
            if (checkSecure(temp->m_floorNum)) {
                // Skip this secured floor                                                                                                                                        
                temp = temp->m_next;
                continue; // Skip to the next floor                                                                                                                               
            }

            // If the floor is not secured, process it                                                                                                                            
            m_currentFloor = temp;
            m_downRequests = temp->m_next;  // Remove the processed floor from the list                                                                                           
            m_moveState = DOWN;  // Elevator is moving down                                                                                                                       
            m_isIdle=false;
            return true;
        }
    }

    // If there are no more valid requests, set the elevator to idle state                                                                                                        
    m_moveState = IDLE;
    m_isIdle=true;
    return false;
}

void Elevator::sortRequestsAscending() {
    // Implement sorting for the upRequests list in ascending order                                                                                                               
    if (!m_upRequests) return;

    bool swapped;
    do {
        swapped = false;
        Floor* temp = m_upRequests;
        while (temp && temp->m_next) {
            if (temp->m_floorNum > temp->m_next->m_floorNum) {
                // Swap the floors                                                                                                                                                
                int tempFloor = temp->m_floorNum;
                temp->m_floorNum = temp->m_next->m_floorNum;
                temp->m_next->m_floorNum = tempFloor;
                swapped = true;
            }
            temp = temp->m_next;
        }
    } while (swapped);  // Keep swapping until no more swaps are needed                                                                                                           
}

void Elevator::sortRequestsDescending() {
    // Implement sorting for the downRequests list in descending order                                                                                                            
    if (!m_downRequests) return;

    bool swapped;
    do {
        swapped = false;
        Floor* temp = m_downRequests;
        while (temp && temp->m_next) {
            if (temp->m_floorNum < temp->m_next->m_floorNum) {
                // Swap the floors                                                                                                                                                
                int tempFloor = temp->m_floorNum;
                temp->m_floorNum = temp->m_next->m_floorNum;
                temp->m_next->m_floorNum = tempFloor;
                swapped = true;
            }
            temp = temp->m_next;
        }
    } while (swapped);  // Keep swapping until no more swaps are needed                                                                                                           
}

void Elevator::enter(int load){
  if (m_load + load <= LOADLIMIT) {
    m_load += load;
  }
}
int Elevator::exit(int load){
  if (m_load - load >= 0) {
    m_load -= load;
  }
  return m_load;
}
void Elevator::dump(){
    if (m_moveState == IDLE) cout << "Elevator " << m_id << " is idle.";
    else if (m_moveState == UP) cout << "Elevator " << m_id << " is moving up.";
    else if (m_moveState == DOWN) cout << "Elevator " << m_id << " is moving down.";
    cout << endl;
    if (m_emergency == true) cout << "someone pushed the emergency button!" << endl;
    if (m_top != nullptr){
        Floor *temp = m_top;
        cout << "Top" << endl;
        while(temp->m_previous != nullptr){
            cout << temp->m_floorNum;
            if (temp->m_floorNum == m_currentFloor->m_floorNum) cout << " current ";
            if (temp->m_secured == true) cout << " secured ";
            cout << endl;
            temp = temp->m_previous;
        }
        cout << temp->m_floorNum;
        if (temp->m_floorNum == m_currentFloor->m_floorNum) cout << " current ";
        if (temp->m_secured == true) cout << " secured ";
        cout << endl;
        cout << "Bottom" << endl;
    }
}









