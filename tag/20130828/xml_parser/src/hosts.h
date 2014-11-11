#include <vector>
class Host{
	public:
		float lat;
		float lng;
		int id;
		float create_time;
		std::vector<float> trig_time;
		std::vector<float> trig_fmag;
		Host();			
};
