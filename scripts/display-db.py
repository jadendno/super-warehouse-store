import sqlite3
import os

def display_table(cursor, table_name: str):
    print(f"\n--- Table: {table_name} ---")
    
    # Fetch column names
    cursor.execute(f"PRAGMA table_info({table_name});")
    columns = [info[1] for info in cursor.fetchall()]
    print(" | ".join(columns))
    
    # Fetch all rows
    cursor.execute(f"SELECT * FROM {table_name};")
    rows = cursor.fetchall()
    for row in rows:
        print(" | ".join(str(item) for item in row))
    
def main():
    db_file = input("Enter the relative path of the SQLite database file: ").strip()
    if not os.path.isfile(db_file):
        print(f"Error: Database file '{db_file}' not found.")
        return
    
    conn = sqlite3.connect(db_file)
    cursor = conn.cursor()
    
    # Get list of all tables in the database
    cursor.execute("SELECT name FROM sqlite_master WHERE type='table';")
    tables = cursor.fetchall()
    
    if not tables:
        print("No tables found in the database.")
    else:
        for (table_name,) in tables:
            display_table(cursor, table_name)
    
    conn.close()

if __name__ == "__main__":
    main()
