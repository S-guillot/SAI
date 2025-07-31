-- Create database sai if it doesn't exist
DO
$do$
BEGIN
   IF NOT EXISTS (
      SELECT FROM pg_catalog.pg_database 
      WHERE datname = 'sai'
   ) THEN
      PERFORM dblink_exec('dbname=' || current_database(), 'CREATE DATABASE sai');
   END IF;
END
$do$;