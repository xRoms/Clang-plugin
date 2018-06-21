#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/AST/AST.h"
#include "clang/AST/Stmt.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Sema/Sema.h"
#include "llvm/Support/raw_ostream.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/ARCMigrate/ARCMT.h"
#include "/usr/local/lib/llvm/tools/clang/lib/ARCMigrate/Transforms.h"
using namespace clang;

namespace {


Rewriter TheRewriter;
int counter = 0;
SourceLocation definecnts;
bool f = false;
std::string prefix = "";
std::vector<SourceLocation> insertcounters;
std::vector<SourceLocation> insertoutput;
std::vector<bool> brace;
bool isnowmain = false;
ASTContext* context;

class FuncDeclVisitor : public RecursiveASTVisitor<FuncDeclVisitor> {

public:
  FuncDeclVisitor() {}

  bool VisitDecl(const Decl *state) {
    if (!f) {
      f = true;
      definecnts = state->getLocStart();
    }
    return true;
  }

	bool VisitReturnStmt( const ReturnStmt *state) {
		if (!isnowmain) {
			return true;
		}
		insertoutput.push_back(state->getLocStart());
		return true;
	}


	bool VisitWhileStmt(const WhileStmt *state) {
		return doCycleThings(state->getBody());
	}

	bool VisitDoStmt(const DoStmt *state) {
		return doCycleThings(state->getBody());
	}

	bool VisitForStmt(const ForStmt *state) {
		return doCycleThings(state->getBody());
	}

	bool VisitCXXForRangeStmt(const CXXForRangeStmt *state) {
		return doCycleThings(state->getBody());
	}


  bool VisitFunctionDecl(const FunctionDecl *funcDecl) {
  	isnowmain = false;
  	if (funcDecl->isMain()) {
  		isnowmain = true;
  		insertoutput.push_back(funcDecl->getLocEnd());
  	}

    const Stmt *location = funcDecl->getBody();
    counter++;
    insertcounters.push_back(location->getLocStart());
	brace.push_back(false);
  	return true;
  }


	bool VisitVarDecl(VarDecl *var) {
		if (prefix < var->getName()) {
			prefix = var->getName();
		}
		return true;
	}


private:
	bool doCycleThings(const Stmt* somest) {
	    counter++;
	    if (isa<CompoundStmt>(somest)) {
	    	insertcounters.push_back(somest->getLocStart());
	    	brace.push_back(false);
	    }
	    else {
	    	insertcounters.push_back(somest->getLocStart());
	    	brace.push_back(true);
	    	SourceLocation q = clang::arcmt::trans::findSemiAfterLocation(somest->getLocEnd(), *context);
	    	if (!q.isValid()) {
	    		q = somest->getLocEnd().getLocWithOffset(1);
	    	}
	    	TheRewriter.InsertTextAfterToken(q, "\n}");
	    }
	    return true;
	}

};


class PrintFunctionsConsumer : public ASTConsumer {

public:
	  PrintFunctionsConsumer(CompilerInstance &Inst, std::string filename): Instance(Inst), manager(Inst.getSourceManager()), visit(FuncDeclVisitor()), fmanager(Inst.getFileManager()) {
      	this->filename = filename;
      	context = &Inst.getASTContext();
      	TheRewriter.setSourceMgr(manager, Inst.getLangOpts());
      }

  virtual bool HandleTopLevelDecl(DeclGroupRef dg) override {
    for (Decl *decl : dg) {
      if (manager.isInMainFile(decl->getLocation())) {
        visit.TraverseDecl(decl);
 	  }
    }

    return true;
  }

	  ~PrintFunctionsConsumer() {
	  	std::string cntname = prefix + "counter";
		for (int i = 1; i <= counter; i++) {
			TheRewriter.InsertTextBefore(definecnts, "long long " + cntname + std::to_string(i) + " = 0;\n");
		}
		int cnt = 0;
		auto jp = brace.begin();
		for (auto i : insertcounters) {
			cnt++;
			if (*jp) {
				TheRewriter.InsertTextBefore(i, "{\n" + cntname + std::to_string(cnt) + "++;\n");
			}
			else {
				TheRewriter.InsertTextAfterToken(i, "\n" + cntname + std::to_string(cnt) + "++;\n");
			}
			jp++;
		}

		for (auto i: insertoutput) {
			std::string insert = "";
			if (filename != "") {
				insert += "freopen(\"" + filename + "\", \"w\", stdout);\n";
			}
			for (int j = 1; j <= counter; j++) {
				insert += "cout << " + cntname + std::to_string(j) + " <<  \" \";\n";
			}
			TheRewriter.InsertTextBefore(i, insert);
		}
		std::error_code EC;
		raw_ostream *out = new llvm::raw_fd_ostream("temp.cpp", EC, llvm::sys::fs::F_None);

		for (Rewriter::buffer_iterator i = TheRewriter.buffer_begin(); i != TheRewriter.buffer_end(); i++) {
			auto st = i->second.begin();
			auto en = i->second.end();
			*out << std::string(st, en);
			out->flush();
		}	
	}
private:
	FuncDeclVisitor visit;	
	SourceManager &manager;	
	CompilerInstance &Instance;
	FileManager &fmanager;
	std::string filename;

};

class PrintFunctionNamesAction : public PluginASTAction {
private:
	std::string filename = "";

protected:
	std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, llvm::StringRef) override {
		return llvm::make_unique<PrintFunctionsConsumer>(CI, filename);
	}

	bool ParseArgs(const CompilerInstance &CI, const std::vector<std::string> &args) override {
		if (args.size() > 1) {
			llvm::outs() << "Usage: start.sh <counters output file (if needed)>";
			return false;
		}
		if (args.size() == 1) {
			filename = args[0];
		}
		return true;
	}

};

}

static FrontendPluginRegistry::Add<PrintFunctionNamesAction>
X("print-fns", "print function names");
