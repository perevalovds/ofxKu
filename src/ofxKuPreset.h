#pragma once

//Presets maintaining system

#include "ofMain.h"


struct ofxKuPreset {
    ofxKuPreset();
    void add_int(string name, int *var, int defValue);
	void add_float(string name, float *var, float defValue);

	bool load(string file_name);
	void save(string file_name);
	void load();
	void save();

	void store(int id);
	void recall(int id);		//one-step transition to preset
	void trans_to(int id, float trans_time); //slow transition between presets
	void stop_transition();
	void update();
	void update_manual(float t);	//instead update, call this for manual morphing between presets, t==0 - start, t==1 - end

	bool trans_is_going();	//now is transition
	float trans_value();	//0 - start, ... 1 - end

	struct Var {
		string name;
		int *vari = nullptr;
		float *varf = nullptr;

		int defaulti = 0;
		float defaultf = 0.;
		//for transition
		double trans0 = 0;
		double trans1 = 0;	

		Var(string name0, int *vari0, int defValuei) {
			name = name0;
			vari = vari0;
			varf = 0;
			trans0 = *vari;
			trans1 = *vari;

			defaulti = defValuei;
		}
		Var(string name0, float *varf0, float defValuef) {
			name = name0;
			vari = 0;
			varf = varf0;
			trans0 = *varf;
			trans1 = *varf;

			defaultf = defValuef;
		}
		double value() {
			if (varf) return *varf;
			if (vari) return *vari;
			return 0;
		}
		void set_value(double v) {
			if (varf) *varf = v;
			if (vari) *vari = v;
		}
		void set_default() {
			if (varf) *varf = defaultf;
			if (vari) *vari = defaulti;
		}
		void trans_start(double to) {
			trans0 = value();
			trans1 = to;
		}
		void trans(float t) {
			if (varf) {
				//t = ofMap(sin(ofMap(t,0,1,-PI/2,PI/2)),-1,1,0,1);
				//t = t*t*(3-2*t);
				*varf = ofLerp(trans0, trans1, t);
			}
			if (vari) *vari = (t<0.5)?trans0:trans1;
		}

	};

private:
	vector<Var> var_;
	vector<string> preset_;
	string file_name_;

	Var *findVar(const string &name);
	unordered_map<string, int> var_map_;
	void name_to_map(string name, int i);


	bool trans_;
	float trans_time_;

	float trans_start_;
	float trans_value_;
};


