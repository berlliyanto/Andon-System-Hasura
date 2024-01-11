void p10State(const char* state, const char* type){
  if(strcmp(state, "call") == 0){
    displayCalling(type);
  } else if(strcmp(state, "standby") == 0){
    displayStandby();
  } else if(strcmp(state, "running") == 0){
    const char* hopper = "";
    if(proximityState == 0){
      hopper = "OK";
    } else {
      hopper = "LOW";
    }
    displayRunning(shotCounter, hopper);
  } else if (strcmp(state, "change") == 0) {
    displayChange(type);
  }
}