/*
Consider telephone book database of N clients. Make use of a hash
table implementation to quickly look up client’s telephone number.
Make use of two collision handling techniques and compare them using
number of comparisons required to find a set of telephone numbers
(Note: Use linear probing with replacement and without replacement)
 */

#include <iostream>
#include <ios>
#include <cstring>
using namespace std;

// Represents an entry in the HashTable
// Each key will be mapped to a Record
class Record {

	char clientName[ 20 ] ;
	int telephone ;
	int chain ;

public:

	Record() {
		// Created empty record with default values
		strcpy( clientName , "----" ) ;
		telephone = 0 ;
		chain = -1 ;
	}

	Record( char name[] , int telephone ) {
		strcpy( clientName , name ) ;
		this -> telephone = telephone ;
		this -> chain = -1 ;
	}

	void printRecord() {
		cout.width( 20 ) ;
		cout << clientName << " " ;
		cout.width( 20 ) ;
		cout << telephone << "" ;
		cout.width( 20 ) ;
		cout << chain << "\n" ;
	}

	bool isEmpty() {
		// Check if current record is default or populated
		return telephone == 0 ;
	}

	friend class HashTable ;
};

class HashTable {

	int tableSize ;
	Record* records;

	// Hash function which maps clientName to a unique integer
	// Defined by hash( clientName ) = SUM mod TABLESIZE
	// where  SUM -> Sum of all ASCII values of chars in clientName
	//        TABLESIZE -> Size of the HashTable
	int hash( char clientName[] ) {
		int asciiSum = 0 ;
		char c = clientName[0] ;
		for( int i = 0 ; i < strlen( clientName ) ; i++ ) {
			asciiSum += clientName[i] ;
			c = clientName[++i] ;
		}
		return asciiSum % tableSize ;
	}

	bool checkOriginal( int index ) {
		return hash( records[ index ].clientName ) == index ; 
	}

public:

	HashTable( int tableSize ) {
		this -> tableSize = tableSize ;
		records = new Record[ tableSize ] ;
	}

	void createTable() {
		for( int i = 0 ; i < (this->tableSize) ; i++ ) {
			records[ i ] = Record() ;
		}
	}

	void displayTable() {
		cout.width( 20 ) ;
		cout << "Client Name  " ;
		cout.width( 20 ) ;
		cout << "Telephone  ";
		cout.width( 20 ) ;
		cout << "Chain  " << "\n" ;
		for( int i = 0 ; i < (this->tableSize) ; i++ ) {
			cout << i << " " ;
			records[ i ].printRecord() ;
		}
	}

	void insertRecordNotReplacement( Record newRecord ) {
		int index = hash( newRecord.clientName ) ;
		if( records[ index ].isEmpty() ) {
			records[ index ] = newRecord ;
		}
		else {
			// Handle Collision with linear probing technique
			// Increment a counter `currentIndex` which searchs for the
			// next empty record
			// Chaining without replacement
			int currentIndex = index ;
			while( !records[ currentIndex ].isEmpty() ) {
				currentIndex = ( currentIndex + 1 ) % tableSize ;
			}
			records[ index ].chain = currentIndex ;
			records[ currentIndex ] = newRecord ;
		}
	}

	void insertRecordReplacement( Record newRecord ) {
		int index = hash( newRecord.clientName ) ;
		if( records[ index ].isEmpty() ) {
			// If location at `index` is empty, place record
			records[ index ] = newRecord ;
		}
		else {
			cout << "Collision occurred at loc=" << index << " \n" ; 
			// Check if record at `index` belongs to that position
			if( checkOriginal( index ) )  {
				// The record belongs to `index`, newRecord will be placed at the next 
				// empty location
				int currentIndex = index ;
				while( !records[ currentIndex ].isEmpty() ) {
					currentIndex = ( currentIndex + 1 ) % tableSize ;
				}
				records[ ( currentIndex - 1 ) % tableSize ].chain = currentIndex ;
				records[ currentIndex ] = newRecord ;
			}
			else {
				// The record does not belong to `index`, add `newRecord` to `index`
				// and place `existingRecord` to next empty location
				Record existingRecord = records[ index ] ;
				records[ index ] = newRecord ;
				int currentIndex = index ;
				while( !records[ currentIndex ].isEmpty() ) {
					currentIndex = ( currentIndex + 1 ) % tableSize ;
				}
				records[ index ].chain = currentIndex ;
				records[ currentIndex ] = existingRecord ;
			}
		}
	}

	void searchRecord( char clientName[] ) {
		int index = hash( clientName ) ;
		if( strcmp( clientName , records[ index ].clientName ) == 0  ) {
			records[ index ].printRecord() ;
		}
		else {
			int currentChain = records[ index ].chain ;
			while( records[ currentChain ].chain != -1 ) {
				currentChain = records[ currentChain ].chain ;
			}
			records[ currentChain ].printRecord() ;
		}
	}

};

int main() {
	HashTable table( 10 ) ;
	table.createTable() ;
	table.insertRecordReplacement( Record( "Shubham" , 10002 ) ) ;
	table.displayTable() ;
	table.insertRecordReplacement( Record( "Shubham" , 10003 ) ) ;
	table.displayTable() ;
	table.insertRecordReplacement( Record( "Shubham" , 10004 ) ) ;
	table.displayTable() ;
	table.insertRecordReplacement( Record( "Advait" , 10005 ) ) ;
	table.displayTable() ;
	table.insertRecordReplacement( Record( "Kaustubh" , 10006 ) ) ;
	table.displayTable() ;
	table.insertRecordReplacement( Record( "Swaraj" , 10007 ) ) ;
	table.displayTable() ;
	table.insertRecordReplacement( Record( "Anushka" , 10004 ) ) ;
	table.displayTable() ;
	table.insertRecordReplacement( Record( "PICT" , 10001 ) ) ;
	table.displayTable() ;
	table.insertRecordReplacement( Record( "PICT" , 10001 ) ) ;
	table.displayTable() ;
	table.searchRecord( "Shubham" ) ;
	table.searchRecord( "PICT" ) ;
	table.searchRecord( "Shubham" ) ;
	return 0;
}
