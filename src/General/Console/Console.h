#pragma once

class ConsoleCommand
{
public:
	ConsoleCommand(
		string_view name,
		void (*func)(const vector<string>&),
		int  min_args,
		bool show_in_list = true);
	~ConsoleCommand() = default;

	const string& name() const { return name_; }
	bool          showInList() const { return show_in_list_; }
	void          execute(const vector<string>& args) const;
	size_t        minArgs() const { return min_args_; }

	bool operator<(const ConsoleCommand& c) const { return name_ < c.name(); }
	bool operator>(const ConsoleCommand& c) const { return name_ > c.name(); }

private:
	string name_;
	void (*func_)(const vector<string>&);
	size_t min_args_;
	bool   show_in_list_;
};

class Console
{
public:
	Console()  = default;
	~Console() = default;

	int             numCommands() const { return (int)commands_.size(); }
	ConsoleCommand& command(size_t index);

	void   addCommand(ConsoleCommand& c);
	void   execute(const string& command);
	string lastCommand();
	string prevCommand(int index);
	int    numPrevCommands() const { return cmd_log_.size(); }

private:
	vector<ConsoleCommand> commands_;
	vector<string>         cmd_log_;
};

// Define for neat console command definitions
#define CONSOLE_COMMAND(name, min_args, show_in_list)              \
	void           c_##name(const vector<string>& args);           \
	ConsoleCommand name(#name, &c_##name, min_args, show_in_list); \
	void           c_##name(const vector<string>& args)
