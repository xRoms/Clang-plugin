#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/AST/AST.h"
#include "clang/AST/Stmt.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Sema/Sema.h"
#include "llvm/Support/raw_ostream.h"
#include "clang/Rewrite/Core/Rewriter.h"
using namespace clang;
using namespace std;

namespace {


Rewriter TheRewriter;
int counter = 0;
SourceLocation definecnts;
bool f = false;

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


  bool VisitWhileStmt(const WhileStmt *state) {
    const Stmt * somest = state->getBody();
    counter++;
    if (isa<CompoundStmt>(somest)) {
      TheRewriter.InsertTextAfterToken(somest->getLocStart(),  "\ncounter" + std::to_string(counter) +"++;");
    }
    else {
      TheRewriter.InsertTextAfterToken(state->getLocEnd().getLocWithOffset(1), "\n}");
      TheRewriter.InsertText(somest->getLocStart(), "{ \ncounter" + std::to_string(counter) +"++;\n");
    }
    return true;
  }

  bool VisitDoStmt(const DoStmt *state) {
    const Stmt * somest = state->getBody();
    counter++;
    if (isa<CompoundStmt>(somest)) {
      TheRewriter.InsertTextAfterToken(somest->getLocStart(),  "\ncounter" + std::to_string(counter) +"++;");
    }
    else {
      TheRewriter.InsertTextAfterToken(state->getLocEnd().getLocWithOffset(1), "\n}");
      TheRewriter.InsertText(somest->getLocStart(), "{ \ncounter" + std::to_string(counter) +"++;\n");
    }
    return true;
  }

  bool VisitForStmt(const ForStmt *state) {
    const Stmt * somest = state->getBody();
    counter++;
    if (isa<CompoundStmt>(somest)) {
      TheRewriter.InsertTextAfterToken(somest->getLocStart(),  "\ncounter" + std::to_string(counter) +"++;");
    }
    else {
      TheRewriter.InsertTextAfterToken(state->getLocEnd().getLocWithOffset(1), "\n}");
      TheRewriter.InsertText(somest->getLocStart(), "{ \ncounter" + std::to_string(counter) +"++;\n");
    }
    return true;
  }

  bool VisitCXXForRangeStmt(const CXXForRangeStmt *state) {
    const Stmt * somest = state->getBody();
    counter++;
    if (isa<CompoundStmt>(somest)) {
      TheRewriter.InsertTextAfterToken(somest->getLocStart(),  "\ncounter" + std::to_string(counter) +"++;");
    }
    else {
      TheRewriter.InsertTextAfterToken(state->getLocEnd().getLocWithOffset(1), "\n}");
      TheRewriter.InsertText(somest->getLocStart(), "{ \ncounter" + std::to_string(counter) +"++;\n");
    }
    return true;
  }


  bool VisitFunctionDecl(const FunctionDecl *funcDecl) {
    const Stmt *location = funcDecl->getBody();
    counter++;
  	TheRewriter.InsertTextAfterToken(location->getLocStart(), "\ncounter" + std::to_string(counter) +"++;");
  	llvm::outs().flush();
    return true;
  }
};


class PrintFunctionsConsumer : public ASTConsumer {

public:
	  PrintFunctionsConsumer(CompilerInstance &Inst): Instance(Inst), manager(Inst.getSourceManager()), visit(FuncDeclVisitor()) {
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
      for (int i = 1; i <= counter; i++) {
       TheRewriter.InsertTextBefore(definecnts, "int counter" + std::to_string(i) + " = 0;\n");
      }
	  	for (Rewriter::buffer_iterator i = TheRewriter.buffer_begin(); i != TheRewriter.buffer_end(); i++) {
			auto st = i->second.begin();
			auto en = i->second.end();
			llvm::outs() << std::string(st, en);
	  	}	
	}
private:
	FuncDeclVisitor visit;	
	SourceManager &manager;	
	CompilerInstance &Instance;

};

class PrintFunctionNamesAction : public PluginASTAction {
protected:
  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, llvm::StringRef) override {
    return llvm::make_unique<PrintFunctionsConsumer>(CI);
  }

  bool ParseArgs(const CompilerInstance &CI, const std::vector<std::string> &args) override {
    return true;
  }

};

}

static FrontendPluginRegistry::Add<PrintFunctionNamesAction>
X("print-fns", "print function names");
