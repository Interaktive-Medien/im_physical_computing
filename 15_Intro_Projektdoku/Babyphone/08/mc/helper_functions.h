
#include <Arduino_JSON.h> 





// if the value  in the JSON array received from the database is a string (eg. "19"), we need to convert it into an int
int cast_int(JSONVar idValue) {
    if (JSON.typeof(idValue) == "string") {
        String temp = (const char*)idValue;
        return temp.toInt();
    } 
    // parsing not necessary if the value is already an int
    else if (JSON.typeof(idValue) == "number") {
        return (int)idValue;
    }
    return 0;                                  // Fallback if neither string nor int
}




// Array to store which tracks are allowed (0 = no, 1 = yes)
// We use size 16 to match track numbers 1-15 directly (index 0 is ignored)
int allowed_tracks[16] = {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};






int getRandomAllowedTrack() {
    int validIndices[15]; // Temporary list of tracks marked as '1'
    int count = 0;

    // Check tracks 1 to 15
    for (int i = 1; i <= 15; i++) {
        if (allowed_tracks[i] == 1) {
            validIndices[count] = i;
            count++;
        }
    }

    // If no tracks are allowed, fallback to track 1 or return 0
    if (count == 0) return 1; 

    // Pick a random element from our valid list
    int randomIndex = random(0, count); 
    return validIndices[randomIndex];
}







