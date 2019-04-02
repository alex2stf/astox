#include <astox.h>

const char * errh(const char * msg){
	log_stack("invalid args option %s required", msg);
	std::exit(-1);
	return "";
}

int main(int argc, char* argv[]) {
	using namespace astox;
	using namespace std;

	Value * args = serializers::from_args(argc, argv, Syntax::JS);
	if(args->isArray()){
		log_stack("invalid args");
		std::exit(-1);
	}

	Object * pargs = Object::cast(args);


	Path p(pargs->getWithDefault("-f", errh));
	FileInfo finfo(new Path(p.resolve(FileSystem::getCWD())));


	string fname;
	if(!finfo.exist()){
		printf("%s does not exists", finfo.getPath()->toString().c_str());
	}
	fname = finfo.getPath()->toString();
	string in = util::read_file_to_string(fname);


	lexic lxc = Syntax::JS;
	vector<string_stat> stats;
	scriptengine::strip_comments(in, lxc, &stats);

	string out;
	for(int i = 0; i < stats.size(); i++){
		int s = stats.at(i).stat;
		string cd = (stats.at(i).data);
		stringAlg::trim(cd);
		if(cd.empty()) continue;
		switch(s){
			case COMMENT:
				cd = cd.substr(2, cd.size());
				out.append(pargs->getWithDefault("-clineb", "\n"));
				out.append(cd);
				out.append(pargs->getWithDefault("-clinea", "\n"));
				break;
			case COMMENT_BLOCK:
				cd = cd.substr(2, cd.size());
				cd = cd.substr(0, cd.size() - 1);
				out.append(pargs->getWithDefault("-cblkb", "\n"));
				out.append(cd);
				out.append(pargs->getWithDefault("-cblka", "\n"));
				break;
			case NONE:
				out.append(pargs->getWithDefault("-defb", "\n```\n"));
				out.append(cd);
				out.append(pargs->getWithDefault("-defa", "\n```\n"));
				break;
		}
	}

	Path p2(pargs->getWithDefault("-o", errh));
	util::write_string_to_file(out, p2.resolve(FileSystem::getCWD()));
	return 0;
}
