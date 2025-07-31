#!/bin/bash

# Script to create PostgreSQL database 'sai' and apply schema
# Run this script with: sudo ./setup.sh

echo "Creating PostgreSQL database 'sai'..."

# Create database if it doesn't exist
sudo -u postgres psql -c "SELECT 1 FROM pg_database WHERE datname='sai';" | grep -q 1 || sudo -u postgres createdb sai

if [ $? -eq 0 ]; then
    echo "Database 'sai' created or already exists."
    
    echo "Applying schema..."
    sudo -u postgres psql -d sai -f schema.sql
    
    if [ $? -eq 0 ]; then
        echo "Schema applied successfully!"
        echo "Database 'sai' is ready to use."
    else
        echo "Error applying schema."
        exit 1
    fi
else
    echo "Error creating database."
    exit 1
fi