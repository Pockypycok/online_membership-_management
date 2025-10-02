#!/usr/bin/env bash
set -euo pipefail
make
CSV="subscriber.csv"
{
  echo 1; echo "E2E Tester"; echo "Netflix"; echo "2025-04-01"; echo "1 เดือน";
  echo 2; echo "E2E";
  echo 3; echo "E2E Tester"; echo "E2E Updated"; echo "Disney+"; echo "2025-04-02"; echo "6 เดือน";
  echo 4; echo "E2E Updated";
  echo 5;
  echo 0;
} | ./app "$CSV" | tee e2e_output.txt
