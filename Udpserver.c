
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define MAX 4096

struct Abbreviation {
    const char *short_form;
    const char *meaning;
};

const struct Abbreviation dictionary[] = {
    {"LOL", "laughing out loud"},
    {"LMAO", "laughing my ass off"},
    {"LMFAO", "laughing my freaking ass off"},
    {"ROFL", "rolling on the floor laughing"},
    {"BRB", "be right back"},
    {"BTW", "by the way"},
    {"OMG", "oh my god"},
    {"OMFG", "oh my freaking god"},
    {"IDK", "I don't know"},
    {"IDC", "I don't care"},
    {"TBH", "to be honest"},
    {"TBF", "to be fair"},
    {"IMO", "in my opinion"},
    {"IMHO", "in my humble opinion"},
    {"FYI", "for your information"},
    {"ASAP", "as soon as possible"},
    {"TTYL", "talk to you later"},
    {"GTG", "got to go"},
    {"G2G", "got to go"},
    {"BBL", "be back later"},
    {"BBS", "be back soon"},
    {"RN", "right now"},
    {"NVM", "never mind"},
    {"NGL", "not gonna lie"},
    {"IKR", "I know right"},
    {"SMH", "shaking my head"},
    {"JK", "just kidding"},
    {"LMK", "let me know"},
    {"DM", "direct message"},
    {"IRL", "in real life"},
    {"AFK", "away from keyboard"},
    {"FOMO", "fear of missing out"},
    {"YOLO", "you only live once"},
    {"AFAIK", "as far as I know"},
    {"AFAICT", "as far as I can tell"},
    {"WDYT", "what do you think"},
    {"WYD", "what are you doing"},
    {"HBU", "how about you"},
    {"WBU", "what about you"},
    {"TMI", "too much information"},
    {"OMW", "on my way"},
    {"ETA", "estimated time of arrival"},
    {"NP", "no problem"},
    {"NBD", "no big deal"},
    {"TY", "thank you"},
    {"THX", "thanks"},
    {"TYSM", "thank you so much"},
    {"YW", "you're welcome"},
    {"PLS", "please"},
    {"PLZ", "please"},
    {"FWIW", "for what it's worth"},
    {"ICYMI", "in case you missed it"},
    {"TLDR", "too long didn't read"},
    {"POV", "point of view"},
    {"FAQ", "frequently asked questions"},
    {"DIY", "do it yourself"},
    {"AKA", "also known as"},
    {"TBD", "to be decided"},
    {"TBA", "to be announced"},
    {"TGIF", "thank god it's Friday"},
    {"RIP", "rest in peace"},
    {"RSVP", "please respond"},
    {"BFF", "best friends forever"},
    {"BF", "boyfriend"},
    {"GF", "girlfriend"},
    {"ILY", "I love you"},
    {"ILU", "I love you"},
    {"ILYSM", "I love you so much"},
    {"XOXO", "hugs and kisses"},
    {"CU", "see you"},
    {"CYA", "see you"},
    {"BC", "because"},
    {"B4", "before"},
    {"U", "you"},
    {"UR", "your"},
    {"R", "are"},
    {"Y", "why"},
    {"K", "okay"},
    {"THO", "though"},
    {"MSG", "message"},
    {"TXT", "text"},
    {"PPL", "people"},
    {"PROB", "probably"},
    {"OBV", "obviously"},
    {"SRS", "serious"},
    {"TTYS", "talk to you soon"},
    {"FR", "for real"},
    {"FRFR", "for real for real"},
    {"ISTG", "I swear to god"},
    {"ONG", "on god"},
    {"ATP", "at this point"},
    {"ICL", "I can't lie"},
    {"WTF", "what the heck"},
    {"WTH", "what the heck"},
    {"L8R", "later"},
    {"M8", "mate"},
    {"GR8", "great"},
    {"BRT", "be right there"},
    {"OML", "oh my lord"},
    {"JIC", "just in case"},
    {"WTV", "whatever"},
    {"GG", "good game"},
    {"GL", "good luck"},
    {"HF", "have fun"},
    {"GLHF", "good luck have fun"},
    {"WP", "well played"},
    {"GJ", "good job"},
    {"OOMF", "one of my followers"},
    {"TIL", "today I learned"},
    {"ELI5", "explain like I'm five"},
    {"IYKYK", "if you know you know"},
    {"FTW", "for the win"},
    {"TIA", "thanks in advance"},
    {"PFA", "please find attached"},
    {"IDC", "I don't care"},
    {"B4N", "bye for now"},
    {"NTH", "nothing"},
    {"W/", "with"},
    {"W/O", "without"},
    {"ASL", "age sex location"},
    {"BTS", "behind the scenes"},
    {"CEO", "chief executive officer"},
    {"BRUH", "bro"},
    {"SUS", "suspicious"},
    {"GOAT", "greatest of all time"},
    {"AF", "as heck"},
    {"ISTG", "I swear to god"},
    {"DM me", "direct message me"},
    {"IDC", "I don't care"},
    {"OFC", "of course"},
    {"IIRC", "if I remember correctly"},
    {"IRL", "in real life"},
    {"LMFAO", "laughing my freaking ass off"},
    {"MFW", "my face when"},
    {"MRW", "my reaction when"},
    {"NSFW", "not safe for work"},
    {"SFW", "safe for work"},
    {"TBT", "throwback Thursday"},
    {"ICYDK", "in case you didn't know"},
    {"WIP", "work in progress"},
    {"ETA", "estimated time of arrival"},
    {"FYA", "for your action"},
    {"P.S.", "postscript"},
    {"e.g.", "for example"},
    {"i.e.", "that is"},
    {"etc", "and so on"}
};

#define DICTIONARY_SIZE (sizeof(dictionary) / sizeof(dictionary[0]))

const char *translate_word(const char *word)
{
    for (size_t i = 0; i < DICTIONARY_SIZE; i++) {
        if (strcasecmp(word, dictionary[i].short_form) == 0)
            return dictionary[i].meaning;
    }
    return word;
}

void translate(char *input, char *output, size_t output_size)
{
    char temp[MAX];
    char *token;
    size_t used = 0;

    snprintf(temp, sizeof(temp), "%s", input);
    output[0] = '\0';

    token = strtok(temp, " \t\r\n");

    while (token != NULL) {
        const char *meaning = translate_word(token);
        size_t remaining = output_size - used;
        int written = snprintf(output + used, remaining,
                               "%s%s", used ? " " : "", meaning);

        if (written < 0 || (size_t)written >= remaining)
            break;

        used += (size_t)written;
        token = strtok(NULL, " \t\r\n");
    }
}

int main(void)
{
    int sockfd;
    char buffer[MAX];
    char translated[MAX];
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr *)&server_addr,
             sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        return 1;
    }

    printf("UDP Server Running on port %d...\n", PORT);

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        memset(translated, 0, sizeof(translated));
        client_len = sizeof(client_addr);

        ssize_t received = recvfrom(sockfd, buffer, MAX - 1, 0,
                                    (struct sockaddr *)&client_addr,
                                    &client_len);

        if (received < 0) {
            perror("Receive failed");
            continue;
        }

        buffer[received] = '\0';
        printf("Received: %s\n", buffer);

        translate(buffer, translated, sizeof(translated));

        if (sendto(sockfd, translated, strlen(translated) + 1, 0,
                   (struct sockaddr *)&client_addr,
                   client_len) < 0) {
            perror("Send failed");
            continue;
        }

        printf("Translated: %s\n", translated);
        printf("Translated message sent.\n");
    }

    close(sockfd);
    return 0;
}
