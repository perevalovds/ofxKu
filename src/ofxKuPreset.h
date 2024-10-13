#pragma once

//Presets maintaining system

#include "ofMain.h"


struct ofxKuPreset {
    void add_int(const string& name, int* var, int defValue);
	void add_float(const string& name, float* var, float defValue);

	// Note: string values can't contain spaces - ' '
	void add_string(const string& name, string* var, const string& defValue);

	//bool load(string file_name);
	bool loadFromStrings(const vector<string>& lines);
	//void save(string file_name);
	vector<string> saveToStrings();
	//void load();
	//void save();

	// Export/Import a single preset
	string export_to_string(int id);
	void import_from_string(int id, const string& presetLine);

	// Store/Recall a preset
	void store(int id);
	// one-step transition to preset
	// returns false is not all variables were loaded, and not unknown variables were found
	bool recall(int id, bool load_missed_as_defaults = true, bool only_check_errors = false);
	void trans_to(int id, float trans_time); //slow transition between presets
	void stop_transition();
	void update();
	void update_manual(float t);	//instead update, call this for manual morphing between presets, t==0 - start, t==1 - end

	bool trans_is_going();	//now is transition
	float trans_value();	//0 - start, ... 1 - end

	struct Var {
		string name;
		bool is_string() { 
			return vars != nullptr;
		}
		int *vari = nullptr;
		float *varf = nullptr;
		string* vars = nullptr;

		int defaulti = 0;
		float defaultf = 0.;
		string defaults = "";
		//for transition
		double trans0 = 0;
		double trans1 = 0;	
		string trans0s = "";
		string trans1s = "";

		Var(string name0, int *vari0, int defValuei) {
			name = name0;
			vari = vari0;
			trans0 = *vari;
			trans1 = *vari;

			defaulti = defValuei;
		}
		Var(string name0, float *varf0, float defValuef) {
			name = name0;
			varf = varf0;
			trans0 = *varf;
			trans1 = *varf;

			defaultf = defValuef;
		}
		Var(string name0, string* vars0, const string& defValues) {
			name = name0;
			vars = vars0;
			trans0s = *vars;
			trans1s = *vars;

			defaults = defValues;
		}
		double value() {
			if (varf) return *varf;
			if (vari) return *vari;
			return 0;
		}
		string value_str() {
			if (varf) return ofToString(*varf);
			if (vari) return ofToString(*vari);
			if (vars) return *vars;
			return "?";
		}
		void set_value(const string& v) {
			if (varf) *varf = ofToFloat(v);
			if (vari) *vari = ofToInt(v);
			if (vars) *vars = v;
		}
		void set_default() {
			if (varf) *varf = defaultf;
			if (vari) *vari = defaulti;
			if (vars) *vars = defaults;
		}
		void trans_start_to_default() {
			if (is_string()) {
				trans0s = value_str();
				trans1s = defaults;
			}
			else {
				trans0 = value();
				if (varf) {
					trans1 = defaultf;
				}
				else if (vari) {
					trans1 = defaulti;
				}
			}
		}
			
		void trans_start(const string& to) {
			if (is_string()) {
				trans0s = value_str();
				trans1s = to;
			}
			else {
				trans0 = value();
				trans1 = ofToDouble(to);
			}
		}
		void trans(float t) {
			if (varf) {
				//t = ofMap(sin(ofMap(t,0,1,-PI/2,PI/2)),-1,1,0,1);
				//t = t*t*(3-2*t);
				*varf = ofLerp(trans0, trans1, t);
			}
			if (vari) *vari = (t < 0.5) ? trans0 : trans1;
			if (vars) *vars = (t < 0.5) ? trans0s : trans1s;
		}

	};

private:
	vector<Var> var_;
	vector<string> preset_;
	string file_name_;

	Var *findVar(const string &name);
	unordered_map<string, int> var_map_;
	void name_to_map(string name, int i);


	bool trans_ = false;
	float trans_time_ = 0.5f;

	float trans_start_ = 0;
	float trans_value_ = 0;
};


