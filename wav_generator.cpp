#include <iostream>
#include <fstream>
#include <stdio.h>
#include <math.h>
#include <cstdlib>

char data[441000]; // The sample data array

void switch_to_little_endian(unsigned char *address, int num){
  address[0] = (num & 0xff);
  address[1] = (num >> 8) & 0xff;
  address[2] = (num >> 16) & 0xff;
  address[3] = (num >> 24) & 0xff;
}

int main(int argc, char *argv[]) {
  int sampleRate = 44100; // Sample rate Hz
  int freq = 440; // Quality
  double duration = 0.5; // Length of tone
  int noChannels = 1; // Mono / Stereo
  int bitsPerSample = 16; // Bit per sample
  char fileName[36]; // File name
  unsigned char waveHeader[44] = {0}; // wav header


   if (argc > 2) {
    char *freqArg = argv[2];
    int i = 0;

    if (freqArg[0] == '-') {
       std::cout << "The frequency of a wave file can not be negative" << std::endl;
      return 1;
    }

    while (freqArg[i] != '\0') {
      freq = freq * 10 + (freqArg[i] - '0');
      i++;
    }
  }

  if (argc > 3) {
    const char *durationArg = argv[3];
    duration = std::atof(durationArg);
  }

  // Add RIFF
  waveHeader[0] = 'R';
  waveHeader[1] = 'I';
  waveHeader[2] = 'F';
  waveHeader[3] = 'F';

  // Add format
  waveHeader[8] = 'W';
  waveHeader[9] = 'A';
  waveHeader[10] = 'V';
  waveHeader[11] = 'E';

  // Add fmt
  waveHeader[12] = 'f';
  waveHeader[13] = 'm';
  waveHeader[14] = 't';
  waveHeader[15] = ' ';

  // Add sub chunk size
  waveHeader[16] = 16;
  waveHeader[17] = 0;
  waveHeader[18] = 0;
  waveHeader[19] = 0;

  // Add audio format
  waveHeader[20] = 0x01;
  waveHeader[21] = 0x00;

  // Add num channels
  waveHeader[22] = (noChannels & 0xff);
  waveHeader[23] = (noChannels >> 8) & 0xff;

  // Add sample rate
  waveHeader[24] = (sampleRate & 0xff);
  waveHeader[25] = (sampleRate >> 8) & 0xff;
  waveHeader[26] = (sampleRate >> 16) & 0xff;
  waveHeader[27] = (sampleRate >> 24) & 0xff;

  // Add byte rate
  int byteRate = sampleRate * noChannels * (bitsPerSample / 8);
  waveHeader[28] = (byteRate & 0xff);
  waveHeader[29] = (byteRate >> 8) & 0xff;
  waveHeader[30] =  (byteRate >> 16) & 0xff;
  waveHeader[31] =  (byteRate >> 24) & 0xff;


  // Add block align
  int blockAlign = noChannels * (bitsPerSample / 8);
  waveHeader[32] = (blockAlign & 0xff);
  waveHeader[33] = (blockAlign >> 8) & 0xff;

  // Add bit per sample
  waveHeader[34] = (bitsPerSample & 0xff);
  waveHeader[35] = (bitsPerSample >> 8) & 0xff;

  // Add sub chunk 2 ID
  waveHeader[36] = 'd';
  waveHeader[37] = 'a';
  waveHeader[38] = 't';
  waveHeader[39] = 'a';

  // Add sub chunk 2 size
  int numSamples = duration * sampleRate;
  int subChunk2ID = numSamples * noChannels * (bitsPerSample / 8);
  waveHeader[40] = (subChunk2ID & 0xff);
  waveHeader[41] = (subChunk2ID >> 8) & 0xff;
  waveHeader[42] = (subChunk2ID >> 16) & 0xff;
  waveHeader[43] = (subChunk2ID >> 24) & 0xff;

  // Add Chunk Size
  int chunkSize =  4 + (8 + 16 ) + (8 + subChunk2ID);
  switch_to_little_endian(waveHeader + 4 ,chunkSize);

  // Add the data
  int pos = 0; // used to track byte position of the data array

  // Add the samples to the data array
  for (int i = 0; i < numSamples; i++) {

    // Set the sample to scale to 16-bit audio
    int scaleTo16Bit = 32767;
    short int sample = sin(freq * 2 * M_PI * i / sampleRate) * scaleTo16Bit;

    // Adds the sample to the data
    data[pos] = sample & 0xff;
    data[pos + 1] = (sample >> 8) & 0xff;

    pos += 2; // update the position in the data array by 2 bytes
  }

  // Create the file name;
  int fileNameLen = 0;

  if (argv[1] != NULL) { // If there is a file name

    while (argv[1][fileNameLen] != '\0') {
      fileName[fileNameLen] = argv[1][fileNameLen];
      fileNameLen++;
    }
  } else { // If no file name use the default
    char defaultName[8] = "default";

    while(defaultName[fileNameLen] != '\0') {
      fileName[fileNameLen] = defaultName[fileNameLen];
      fileNameLen++;
    }
  }

  // Add file format
  fileName[fileNameLen] = '.';
  fileName[fileNameLen + 1] = 'w';
  fileName[fileNameLen + 2] = 'a';
  fileName[fileNameLen + 3] = 'v';
  fileName[fileNameLen + 4] = '\0';

  // Write to file
  std::ofstream wav(fileName, std::ios::binary);

  // Add header
  wav.write(reinterpret_cast<const char*>(waveHeader), 44);

  // Write sample data
  wav.write(reinterpret_cast<const char*>(data), numSamples * sizeof(short));
  wav.close();

  return 0;
}