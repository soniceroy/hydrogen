#include "xml_test.h"

#include <unistd.h>

#include <hydrogen/basics/drumkit.h>
#include <hydrogen/basics/pattern.h>
#include <hydrogen/basics/instrument.h>
#include <hydrogen/basics/instrument_list.h>
#include <hydrogen/basics/instrument_layer.h>
#include <hydrogen/basics/instrument_component.h>
#include <hydrogen/basics/sample.h>

#include <hydrogen/helpers/filesystem.h>
#include "test_helper.h"

CPPUNIT_TEST_SUITE_REGISTRATION( XmlTest );


static bool check_samples_data( H2Core::Drumkit* dk, bool loaded )
{
	int count = 0;
	H2Core::InstrumentComponent::setMaxLayers( 16 );
	H2Core::InstrumentList* instruments = dk->get_instruments();
	for( int i=0; i<instruments->size(); i++ ) {
		count++;
		H2Core::Instrument* pInstr = ( *instruments )[i];
		for (std::vector<H2Core::InstrumentComponent*>::iterator it = pInstr->get_components()->begin() ; it != pInstr->get_components()->end(); ++it) {
			H2Core::InstrumentComponent* pComponent = *it;
			for ( int nLayer = 0; nLayer < H2Core::InstrumentComponent::getMaxLayers(); nLayer++ ) {
				H2Core::InstrumentLayer* pLayer = pComponent->get_layer( nLayer );
				if( pLayer ) {
					H2Core::Sample* pSample = pLayer->get_sample();
					if( loaded ) {
						if( pSample->get_data_l()==nullptr || pSample->get_data_l()==nullptr ) return false;
					} else {
						if( pSample->get_data_l()!=nullptr || pSample->get_data_l()!=nullptr ) return false;
					}
				}

			}
		}
	}
	return ( count==4 );
}

static void clean_drumkit(const QString& dk)
{
	H2Core::Filesystem::rm( dk );
	H2Core::Filesystem::file_copy( dk + ".bak", dk );
	H2Core::Filesystem::rm( dk + ".bak" );
}

static  bool file_cmp(const QString& f0, const QString& f1, int skip = 0)
{
	QFile file( f0 );
	file.open( QIODevice::ReadOnly );
	QTextStream in0( &file );
	QStringList textOfFile0;
	int n = 0;
	while ( !in0.atEnd() ) {
		QString line = in0.readLine();
		n++;
		if ( n > skip ) textOfFile0.append( line );
	}

	QFile file2( f1 );
	file2.open( QIODevice::ReadOnly );
	QTextStream in1( &file2 );
	QStringList textOfFile1;
	n = 0;
	while (!in1.atEnd()) {
		QString line = in1.readLine();
		n++;
		if ( n > skip ) textOfFile1.append( line );
	}

	if( textOfFile0.size() != textOfFile1.size() ) return false;
	for(int i = 0; i < textOfFile0.size(); i++) {
		if( textOfFile0[i] != textOfFile1[i] ) return false;
	}
	return true;
}

void XmlTest::testDrumkit()
{
	QString dk_path = H2Core::Filesystem::tmp_dir()+"/dk0";

	H2Core::Drumkit* dk0 = nullptr;
	H2Core::Drumkit* dk1 = nullptr;
	H2Core::Drumkit* dk2 = nullptr;

	// load without samples
	dk0 = H2Core::Drumkit::load( H2TEST_FILE( "/drumkits/baseKit") );
	CPPUNIT_ASSERT( dk0!=nullptr );
	CPPUNIT_ASSERT( dk0->samples_loaded()==false );
	CPPUNIT_ASSERT( check_samples_data( dk0, false ) );
	CPPUNIT_ASSERT_EQUAL( 4, dk0->get_instruments()->size() );
	//dk0->dump();

	// manually load samples
	dk0->load_samples();
	CPPUNIT_ASSERT( dk0->samples_loaded()==true );
	CPPUNIT_ASSERT( check_samples_data( dk0, true ) );
	//dk0->dump();

	// load with samples
	dk0 = H2Core::Drumkit::load( H2TEST_FILE( "/drumkits/baseKit" ), true );
	CPPUNIT_ASSERT( dk0!=nullptr );
	CPPUNIT_ASSERT( dk0->samples_loaded()==true );
	CPPUNIT_ASSERT( check_samples_data( dk0, true ) );
	//dk0->dump();

	// unload samples
	dk0->unload_samples();
	CPPUNIT_ASSERT( dk0->samples_loaded()==false );
	CPPUNIT_ASSERT( check_samples_data( dk0, false ) );
	//dk0->dump();

	/*
	// save drumkit elsewhere
	dk0->set_name( "dk0" );
	CPPUNIT_ASSERT( dk0->save( dk_path, false ) );
	CPPUNIT_ASSERT( H2Core::Filesystem::file_readable( dk_path+"/drumkit.xml" ) );
	CPPUNIT_ASSERT( H2Core::Filesystem::file_readable( dk_path+"/crash.wav" ) );
	CPPUNIT_ASSERT( H2Core::Filesystem::file_readable( dk_path+"/hh.wav" ) );
	CPPUNIT_ASSERT( H2Core::Filesystem::file_readable( dk_path+"/kick.wav" ) );
	CPPUNIT_ASSERT( H2Core::Filesystem::file_readable( dk_path+"/snare.wav" ) );
	// load file
	dk1 = H2Core::Drumkit::load_file( dk_path+"/drumkit.xml" );
	CPPUNIT_ASSERT( dk1!=nullptr );
	//dk1->dump();
	// copy constructor
	dk2 = new H2Core::Drumkit( dk1 );
	dk2->set_name( "COPY" );
	CPPUNIT_ASSERT( dk2!=nullptr );
	// save file
	CPPUNIT_ASSERT( dk2->save_file( dk_path+"/drumkit.xml", true ) );
	*/

	delete dk0;
	//delete dk1;
	//delete dk2;
}

//Load drumkit which includes instrument with invalid ADSR values.
// Expected behavior: The drumkit will be loaded successfully.
//					  In addition, the drumkit file will be saved with
//					  correct ADSR values.
void XmlTest::testDrumkit_UpgradeInvalidADSRValues()
{
	H2Core::Drumkit* pDrumkit = nullptr;

	//1. Check, if the drumkit has been loaded
	pDrumkit = H2Core::Drumkit::load( H2TEST_FILE( "/drumkits/invAdsrKit") );
	CPPUNIT_ASSERT( pDrumkit != nullptr );

	//2. Make sure that the instruments of the drumkit have been loaded correctly (see GH issue #839)
	H2Core::InstrumentList* pInstruments = pDrumkit->get_instruments();
	CPPUNIT_ASSERT( pInstruments != nullptr );

	H2Core::Instrument* pFirstInstrument = pInstruments->get(0);
	CPPUNIT_ASSERT( pFirstInstrument != nullptr );

	H2Core::InstrumentLayer* pLayer = pFirstInstrument->get_components()->front()->get_layer(0);
	CPPUNIT_ASSERT( pLayer != nullptr );

	H2Core::Sample* pSample = pLayer->get_sample();
	CPPUNIT_ASSERT( pSample != nullptr );

	CPPUNIT_ASSERT( pSample->get_filename() == QString("snare.wav"));

	//3. Make sure that the original (invalid) file has been saved as a backup
	CPPUNIT_ASSERT( H2Core::Filesystem::file_exists( H2TEST_FILE( "/drumkits/invAdsrKit/drumkit.xml.bak") ) );

	if( pDrumkit ) {
		delete pDrumkit;
	}

	clean_drumkit( H2TEST_FILE( "/drumkits/invAdsrKit/drumkit.xml") );
}

void XmlTest::testDrumkit2008()
{
	H2Core::Drumkit* pDrumkit = nullptr;

	pDrumkit = H2Core::Drumkit::load( H2TEST_FILE( "/drumkits/2008") );
	CPPUNIT_ASSERT( pDrumkit != nullptr );

	H2Core::InstrumentList* pInstruments = pDrumkit->get_instruments();
	CPPUNIT_ASSERT( pInstruments != nullptr );

	CPPUNIT_ASSERT( file_cmp( H2TEST_FILE( "/drumkits/2008/drumkit.xml"), H2TEST_FILE( "/drumkits/2008/drumkit.ref"), 2 ) );

	if( pDrumkit ) {
		delete pDrumkit;
	}

	clean_drumkit( H2TEST_FILE( "/drumkits/2008/drumkit.xml") );
}

void XmlTest::testPattern()
{
	QString pat_path = H2Core::Filesystem::tmp_dir()+"/pat";

	H2Core::Pattern* pat0 = nullptr;
	H2Core::Drumkit* dk0 = nullptr;
	H2Core::InstrumentList* instruments = nullptr;

	dk0 = H2Core::Drumkit::load( H2TEST_FILE( "/drumkits/baseKit" ) );
	CPPUNIT_ASSERT( dk0!=nullptr );
	instruments = dk0->get_instruments();
	CPPUNIT_ASSERT( instruments->size()==4 );

	pat0 = H2Core::Pattern::load_file( H2TEST_FILE( "/pattern/pat.h2pattern" ), instruments );
	CPPUNIT_ASSERT( pat0 );

	pat0->save_file( "dk_name", "author", "license", pat_path );

	delete pat0;
	delete dk0;
}

