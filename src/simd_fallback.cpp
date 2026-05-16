// src/simd_fallback.cpp
// Implementações escalares (não-SIMD) para funções fastp_simd
// Usado quando Google Highway não está disponível no Windows

#include "simd.h"
#include <cctype>
#include <cstring>


namespace fastp_simd {

int countMismatchesBounded(const char *seq1, const char *seq2, int len,
                           int max_mismatch) {
  int mismatches = 0;
  for (int i = 0; i < len; ++i) {
    if (seq1[i] != seq2[i]) {
      mismatches++;
      if (mismatches > max_mismatch)
        return mismatches;
    }
  }
  return mismatches;
}

int countMismatches(const char *seq1, const char *seq2, int len) {
  return countMismatchesBounded(seq1, seq2, len, len);
}

void countQualityMetrics(const char *seq, const char *qual, int len,
                         char threshold, int &low_qual, int &n_base,
                         int &total) {
  low_qual = 0;
  n_base = 0;
  total = len;
  for (int i = 0; i < len; ++i) {
    if (qual[i] < threshold)
      low_qual++;
    if (seq[i] == 'N' || seq[i] == 'n')
      n_base++;
  }
}

int countAdjacentDiffs(const char *seq, int len) {
  int diffs = 0;
  for (int i = 1; i < len; ++i) {
    if (seq[i] != seq[i - 1])
      diffs++;
  }
  return diffs;
}

void reverseComplement(const char *src, char *dest, int len) {
  for (int i = 0; i < len; ++i) {
    char c = src[len - 1 - i];
    switch (c) {
    case 'A':
      dest[i] = 'T';
      break;
    case 'T':
      dest[i] = 'A';
      break;
    case 'C':
      dest[i] = 'G';
      break;
    case 'G':
      dest[i] = 'C';
      break;
    case 'a':
      dest[i] = 't';
      break;
    case 't':
      dest[i] = 'a';
      break;
    case 'c':
      dest[i] = 'g';
      break;
    case 'g':
      dest[i] = 'c';
      break;
    default:
      dest[i] = c;
      break;
    }
  }
}

bool testSimd() {
  // Fallback escalar carregado com sucesso
  return true;
}

} // namespace fastp_simd