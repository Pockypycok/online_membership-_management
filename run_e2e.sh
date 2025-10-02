#!/usr/bin/env bash
set -euo pipefail
make
CSV="data/e2e.csv"
cat > "$CSV" <<EOF
Name, Address, PhoneNumber
E2E One, Street 1, 111
E2E Two, Street 2, 222
EOF

{
  echo 1; echo "E2E Three"; echo "Street 3"; echo "333";
  echo 2; echo "E2E";
  echo 3; echo "333"; echo "E2E ThreeX"; echo "Street 3X"; echo "334";
  echo 4; echo "111";
  echo 5;
  echo 0;
} | ./app "$CSV" | tee e2e_output.txt

echo "---- Summary ----"
echo "Remaining lines in CSV (including header):"
wc -l "$CSV"
echo "Done."
