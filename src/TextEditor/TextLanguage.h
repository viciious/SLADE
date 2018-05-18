#pragma once

namespace ZScript
{
class Function;
class Definitions;
} // namespace ZScript

class TLFunction
{
public:
	struct Parameter
	{
		string type;
		string name;
		string default_value;
		bool   optional;

		void parse(vector<string>& tokens);
	};

	struct Context
	{
		string            context;
		vector<Parameter> params;
		string            return_type;
		string            description;
		string            qualifiers;
		string            deprecated;
		bool              custom = false;

		Context() = default;
		Context(
			string_view       context,
			vector<Parameter> params,
			string_view       return_type = "",
			string_view       description = "",
			string_view       qualifiers  = "",
			string_view       deprecated  = "",
			bool              custom      = false) :
			context{ context.data(), context.size() },
			params{ std::move(params) },
			return_type{ return_type.data(), return_type.size() },
			description{ description.data(), description.size() },
			qualifiers{ qualifiers.data(), qualifiers.size() },
			deprecated{ deprecated.data(), deprecated.size() },
			custom{ custom }
		{
		}
	};

	TLFunction(string_view name = "") : name_{ name.data(), name.size() } {}
	~TLFunction() = default;

	const string&          name() const { return name_; }
	const vector<Context>& contexts() const { return contexts_; }
	Context                context(unsigned index) const;

	void setName(string_view name) { S_SET_VIEW(name_, name); }
	void addContext(
		string_view context,
		string_view args,
		string_view return_type = "void",
		string_view description = "");
	void addContext(const string& context, const ZScript::Function& func, bool custom);

	void clear()
	{
		name_.clear();
		contexts_.clear();
	}
	void clearCustomContexts();

	bool hasContext(string_view name);

private:
	string          name_;
	vector<Context> contexts_;
};

class TextLanguage
{
public:
	enum WordType
	{
		Keyword  = 0,
		Constant = 1,
		Type     = 2,
		Property = 3,
	};

	struct Block
	{
		string start;
		string end;
	};

	TextLanguage(string_view id);
	~TextLanguage();

	const string& id() const { return id_; }
	const string& name() const { return name_; }
	const string& lineComment() const { return line_comment_; }
	const string& commentBegin() const { return comment_begin_; }
	const string& commentEnd() const { return comment_end_; }
	const string& preprocessor() const { return preprocessor_; }
	const string& docComment() const { return doc_comment_; }
	bool          caseSensitive() const { return case_sensitive_; }
	const string& blockBegin() const { return block_begin_; }
	const string& blockEnd() const { return block_end_; }

	const vector<string>& ppBlockBegin() const { return pp_block_begin_; }
	const vector<string>& ppBlockEnd() const { return pp_block_end_; }
	const vector<string>& wordBlockBegin() const { return word_block_begin_; }
	const vector<string>& wordBlockEnd() const { return word_block_end_; }
	const vector<string>& jumpBlocks() const { return jump_blocks_; }
	const vector<string>& jumpBlocksIgnored() const { return jb_ignore_; }

	void copyTo(TextLanguage* copy);

	void setName(string_view name) { S_SET_VIEW(name_, name); }
	void setLineComment(string_view token) { S_SET_VIEW(line_comment_, token); }
	void setCommentBegin(string_view token) { S_SET_VIEW(comment_begin_, token); }
	void setCommentEnd(string_view token) { S_SET_VIEW(comment_end_, token); }
	void setPreprocessor(string_view token) { S_SET_VIEW(preprocessor_, token); }
	void setDocComment(string_view token) { S_SET_VIEW(doc_comment_, token); }
	void setCaseSensitive(bool cs) { case_sensitive_ = cs; }
	void addWord(WordType type, const string& word, bool custom = false);
	void addFunction(
		string_view name,
		string_view args,
		string_view desc        = "",
		bool        replace     = false,
		string_view return_type = "");
	void loadZScript(ZScript::Definitions& defs, bool custom = false);

	vector<string> wordList(WordType type, bool include_custom = true) const;
	string         wordListScintilla(WordType type, bool include_custom = true);
	vector<string> functionsList();
	string         functionsListScintilla();
	string         autocompletionList(string_view start = "", bool include_custom = true);

	wxArrayString wordListSorted(WordType type, bool include_custom = true);
	wxArrayString functionsSorted();

	string wordLink(WordType type) const { return word_lists_[type].lookup_url; }
	string functionLink() const { return f_lookup_url_; }

	bool isWord(WordType type, string_view word);
	bool isFunction(string_view word);

	TLFunction* function(string_view name);

	void clearWordList(WordType type) { word_lists_[type].list.clear(); }
	void clearFunctions() { functions_.clear(); }
	void clearCustomDefs();

	// Static functions
	static bool          readLanguageDefinition(MemChunk& mc, string_view source);
	static bool          loadLanguages();
	static TextLanguage* fromId(string_view id);
	static TextLanguage* fromIndex(unsigned index);
	static TextLanguage* fromName(string_view name);
	static wxArrayString languageNames();

private:
	string         id_;                     // Used internally
	string         name_;                   // The language 'name' (will show up in the language dropdown, etc)
	string         line_comment_   = "//";  // The beginning token for a line comment
	string         comment_begin_  = "/*";  // The beginning token for a block comment
	string         comment_end_    = "*/";  // The ending token for a block comment
	string         preprocessor_   = "#";   // The beginning token for a preprocessor directive
	string         doc_comment_    = "///"; // The beginning token for a 'doc' comment (eg. /// in c/c++)
	bool           case_sensitive_ = true;  // Whether words are case-sensitive
	vector<string> jump_blocks_;            // The keywords to search for when creating jump to list (eg. 'script')
	vector<string> jb_ignore_;              // The keywords to ignore when creating jump to list (eg. 'optional')
	string         block_begin_ = "{";      // The beginning of a block (eg. '{' in c/c++)
	string         block_end_   = "}";      // The end of a block (eg. '}' in c/c++)
	vector<string> pp_block_begin_;         // Preprocessor words to start a folding block (eg. 'ifdef')
	vector<string> pp_block_end_;           // Preprocessor words to end a folding block (eg. 'endif')
	vector<string> word_block_begin_;       // Words to start a folding block (eg. 'do' in lua)
	vector<string> word_block_end_;         // Words to end a folding block (eg. 'end' in lua)

	// Word lists
	struct WordList
	{
		vector<string> list;
		bool           upper = false;
		bool           lower = false;
		bool           caps  = false;
		string         lookup_url;
	};
	WordList word_lists_[4];
	WordList word_lists_custom_[4];

	// Functions
	vector<TLFunction> functions_;
	string             f_lookup_url_;
};
