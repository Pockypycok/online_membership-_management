#!/bin/bash
# Online Membership Management System - End-to-End Test Script

APP=./app

echo "Starting End-to-End test..."
rm -f users.csv deleted_users.csv e2e_output.txt

echo "user_id,name,service,start_date,duration,expiry_date,status,email,phone,last_updated,checksum" > users.csv

echo "Add User 1..."
echo -e "1\nAlice Wonderland\nPremium\n2025-10-09\n12M\n2026-10-09\nActive\nalice@example.com\n0812345678\n0\n" | $APP >> e2e_output.txt

echo "Add User 2..."
echo -e "1\nBob Builder\nStandard\n2025-09-01\n6M\n2026-03-01\nActive\nbob@example.com\n0801112222\n0\n" | $APP >> e2e_output.txt

echo "Display All..."
echo -e "5\n0\n" | $APP >> e2e_output.txt

echo "Edit Alice..."
echo -e "3\nAlice\nAlice Updated\nVIP\n2025-10-10\n24M\n2027-10-10\nActive\nalice_new@example.com\n0999999999\n0\n" | $APP >> e2e_output.txt

echo "Delete Bob..."
echo -e "4\nBob\ny\n0\n" | $APP >> e2e_output.txt

echo "Display After Delete..."
echo -e "5\n0\n" | $APP >> e2e_output.txt

echo "End-to-End test completed."
echo "Output saved to e2e_output.txt"
echo "---------------------------------------------"
tail -n 10 e2e_output.txt
echo "---------------------------------------------"
