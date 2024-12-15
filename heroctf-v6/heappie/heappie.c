#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct Music {
    void (*play)(struct Music*);

    char title[32];
    char artist[32];
    char description[128];
} Music;

Music* playlist = NULL;
int playlist_size = 0;
int playlist_capacity = 0;

void win() {
    char flag[64];
    FILE* f = fopen("flag.txt", "r");
    if (f == NULL) {
        puts("Flag file is missing!");
        exit(1);
    }

    fgets(flag, sizeof(flag), f);
    printf("Flag: %s", flag);
    fclose(f);
}

void play_1(Music* music) {
    printf("Playing music 1: %s by %s\n", music->title, music->artist);
}

void play_2(Music* music) {
    printf("Playing music 2: %s by %s\n", music->title, music->artist);
}

void play_3(Music* music) {
    printf("Playing music 3: %s by %s\n", music->title, music->artist);
}

void* choose_random_play() {
    int choice = rand() % 3;
    switch(choice) {
        case 0:
            return (void*)play_1;
        case 1:
            return (void*)play_2;
        case 2:
            return (void*)play_3;
    }
    return NULL;
}

void show_playlist() {
    if (playlist_size == 0) {
        puts("Your playlist is empty!\n");
        return;
    }

    puts("Your playlist:");
    for (int i = 0; i < playlist_size; i++) {
        Music* music = &playlist[i];
        printf("\t%d. %s by %s (song: %p)\n", i + 1, music->title, music->artist, music->play);
    }
}

void add_music() {

    if (playlist_size == playlist_capacity) {
        playlist_capacity += 10;
        playlist = realloc(playlist, playlist_capacity * sizeof(Music));
    }

    Music* music = &playlist[playlist_size];
    char add_music = 'n';
    printf("Do you want to add a sound to the music? (y/n): ");
    scanf(" %c", &add_music);
    if (add_music == 'y') {
        music->play = choose_random_play();
        puts("Sound added to the music!");
    } else {
        puts("No sound added to the music!");
    }
    printf("Enter music title: ");
    scanf("%31s", music->title);
    printf("Enter music artist: ");
    scanf("%31s", music->artist);
    printf("Enter music description: ");
    scanf("%s", music->description);

    puts("Music added to your playlist!\n");
    playlist_size++;
}

void play_music() {
    int index;

    printf("Enter music index: ");
    scanf("%d", &index);

    if (index < 0 || index >= playlist_size) {
        puts("Invalid index!\n");
        return;
    }

    Music* music = &playlist[index];
    if (music->play == NULL) {
        puts("No sound for this music!\n");
        return;
    }

    music->play(music);
}

void delete_music() {
    int index;
    printf("Enter music index: ");
    scanf("%d", &index);

    if (index < 0 || index >= playlist_size) {
        puts("Invalid index!\n");
        return;
    }

    Music* music = &playlist[index];
    memset(music, 0, sizeof(Music));

    for (int i = index; i < playlist_size - 1; i++) {
        playlist[i] = playlist[i + 1];
    }

    puts("Music deleted from your playlist!\n");
    playlist_size--;
}

void setup() {
    setvbuf(stdout, 0, 2, 0);
    setvbuf(stdin, 0, 2, 0);

    puts("============================================");
    puts("=>           Welcome to Heappie           <=");
    puts("============================================");

    puts("Fill your playlist with your favorite music!");
    puts("");
    puts("Examples:");
    puts("\t1. Imagine by John Lennon");
    puts("\t2. Blowin' in the Wind by Bob Dylan");
    puts("\t3. Aquarius/Let the Sunshine In by The 5th Dimension");
    puts("");
}

int main() {
    srand(time(NULL));
    setup();

    while(1) {
        puts("1. Add music");
        puts("2. Play music");
        puts("3. Delete music");
        puts("4. Show playlist");

        printf(">> ");
        int choice;
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                add_music();
                break;
            case 2:
                play_music();
                break;
            case 3:
                delete_music();
                break;
            case 4:
                show_playlist();
                break;
            default:
                puts("Invalid choice!\n");
                break;
        }
    }

    free(playlist);
    return 0;
}
