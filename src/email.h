#ifndef EMAIL_H
#define EMAIL_H

#include <iostream>
#include <cstdlib>

using namespace std;

const string SENDER = "hpham096@ucr.edu";
const string RECIPIENT = "phry.ham24@gmail.com";
const string SUBJECT = "TEST";
const string body = "testing...........";

const string CREDENTIALS_PATH = "../data/credentials.json";
const string ACCESS_TOKEN = "ya29.a0AfB_byD1nu0rfNHJ1vy_6SkqXobig80OWXDlkluYcc0F6vryzflAfQ2qqeZs3mf2ON_d-XqF7iLKR_SO5IbjKsY2TG_O0PVupRvrfDP_3STin-zeGkAPZGGvEhMghOSfYmtlEPfzNc8aJPAerYmsHq9y18ZLanUaCgYKAU0SARESFQHsvYlszZ6C16Y6p7i1seOEIzEl3A0166";
                             // need to change this on playgrund every 3600 seconds

const string COMMAND = "curl -X POST -H \"Authorization: Bearer " + ACCESS_TOKEN +
                        "\" -H \"Content-Type: application/json\" -d '" +
                        "{ \"raw\": \"RAW_EMAI_CONTENT_BASE64_ENCDED\" }' https://gmail.googleapis.com/gmail/v1/users/me/messages/send";

void sendEmail(){
    int result = system(COMMAND.c_str());
    
    if (result == 0) {
        cout << "Email sent successfully." << endl;
    }
    else {
        cerr << "Failed to send email." << endl;
    }
}

#endif