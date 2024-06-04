bool msg_type(const String& message, const String& type) {
    int index = message.indexOf(type);
    return index != -1;
}

// Define keywords and corresponding variable to store values
const String keywords[] = {"LDR:", "pump:", "tempe:", "hum:", "light:", "curt:", "water:"};
int msg_LDR = 0, msg_pump = 1, msg_tempe = 0, msg_hum = 0, msg_light = 1500, msg_curt = 0, msg_water = 0; // Corresponding variables for each sensor

void check_msg(const String& inputString) {
    for (int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); ++i) {
        const String& keyword = keywords[i];
        int index = inputString.indexOf(keyword);

        if (index != -1) {
            // Calculate start position of the number
            int numStartIndex = index + keyword.length();
            while (numStartIndex < inputString.length() && inputString[numStartIndex] == ' ') {
                numStartIndex++; // Skip any spaces directly after the keyword
            }

            // Extract the substring from where the number starts and convert the substring to an integer
            int value = String(inputString.substring(numStartIndex)).toInt();

            // Update the corresponding variable
            if (keyword == "LDR:") {
                msg_LDR = value;
            }
            else if (keyword == "pump:") {
                msg_pump = value;
            }
            else if (keyword == "tempe:") {
                msg_tempe = value;
            }
            else if (keyword == "hum:") {
                msg_hum = value;
            }
            else if (keyword == "light:") {
                msg_light = value;
            }
            else if (keyword == "curt:") {
                msg_curt = value;
            }
            else if (keyword == "water:") {
                msg_water = value;
            }
        }
    }
}