#include <processing/processingmodule.h>
#include <defines.h>
#include <mgr/mgrlog.h>
#include <mgr/mgrrpc.h>
#include <mgr/mgrnet.h>
#include <sstream>
#include <processing/domain_common.h>
#include <fstream>

#define BINARY_NAME "x"
#define PORT 443

MODULE("processing");

namespace processing {
	
using namespace std;

class X : public Module
{
public:
	X() : Module (BINARY_NAME) {
		log.rdbuf()->pubsetbuf(0, 0);
		log.open("/tmp/pmx.log", std::ofstream::out | std::ofstream::app);
		log << "X()" << endl;
	}
	
	virtual ~X() override {
		log.close();
	}
	
	virtual void Open(const int iid) override {
		log << "Open()" << endl;
	}

	virtual void Resume(const int iid) override {
		log << "Resume()" << endl;
	}

	virtual void Suspend(const int iid) override {
		log << "Suspend()" << endl;
	}

	virtual void Close(const int iid) override {
		log << "Close()" << endl;
	}

	virtual void SetParam(const int iid) override {
		log << "SetParam()" << endl;
	}
	
	virtual mgr_xml::Xml Features() override {
		log << "Features" << endl;
		
		mgr_xml::Xml xml;
		auto itemtypes = xml.GetRoot().AppendChild("itemtypes");
		itemtypes.AppendChild("itemtype").SetProp("name", "domain");

		auto features = xml.GetRoot().AppendChild("features");
		features.AppendChild("feature").SetProp("name", PROCESSING_PRICELIST_IMPORT);

		return xml;
	}
	
	virtual mgr_xml::Xml ImportPriceList(const int moduleId, const string& sub_command, const int id) override {
		log << "ImportPriceList(): " << endl
			<< "  moduleId: " << moduleId << endl
			<< "  sub_command: " << sub_command << endl
			<< "  id: " << id << endl;
		
		if (sub_command == "available") {
			return getAvailablePlans();
		}
		
		return mgr_xml::Xml();
	}
	
private:
	ofstream log;
	
	mgr_xml::Xml getAvailablePlans() {
		mgr_xml::Xml availablePlans;
		auto root = availablePlans.GetRoot();
		
		stringstream planName;
		for (int planId = 1; planId <= 10; ++planId) {
			addPlan(root, planId);
		}
		
		log << "## getAvailablePlans()" << endl
			<< availablePlans.Str(true)
			<< "-------------------------" << endl;
		
		return availablePlans;
	}
	
	void addPlan(mgr_xml::XmlNode &pricelist, const int id) {
		auto elem = pricelist.AppendChild("elem");
		
		{
			stringstream idStr;
			idStr << id;
			elem.AppendChild("id").SetContent(idStr.str());
		}
		
		{
			stringstream nameStream;
			nameStream << "dummy plan " << id;
			auto name = nameStream.str();
			elem.AppendChild("name").SetContent(name);
			elem.AppendChild("name_en").SetContent(name);
			elem.AppendChild("name_ru").SetContent(name);
		}
		
		elem.AppendChild("itemtype").SetContent("domain");
	}
};

}; // namespace processing

RUN_MODULE(processing::X)
