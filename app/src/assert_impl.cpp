#include "assert_impl.h"

namespace
{

class MessageBoxThread final
{
public:
	MessageBoxThread(const std::string& text)
		: m_text(text)
		, m_result(IDRETRY)
	{
	}

	int askUser()
	{
		std::thread messageBoxThread = std::thread(&MessageBoxThread::run, this);

		messageBoxThread.join();

		return m_result;
	}

protected:

	void run()
	{

#if !defined(NDEBUG)
		long messageBoxType = MB_RETRYCANCEL;
#else
		long messageBoxType = MB_OK;
#endif

		messageBoxType = messageBoxType | MB_ICONSTOP | MB_TOPMOST;

		m_result = MessageBoxA(0, m_text.c_str(), "Debug Message!", messageBoxType);
	}

private:
	int m_result;

	const std::string m_text;
};

}

namespace Test
{

void doAssert(const char* file, int line, const char* function, const char* expression) noexcept
{
	std::stringstream text;

	text << std::string("Debug Assertion") << std::endl << std::endl;
	text << std::string("Thread: ") << std::this_thread::get_id() << std::endl;
	text << std::string("Function: ") << function << std::endl;
	text << std::string("File: ") << file << std::endl;
	text << std::string("Line: ") << line << std::endl << std::endl;
	text << std::string("Expression: ") << expression << std::endl << std::endl;
	text << std::string("Press Retry to debug the application");

	// Run message box in background thread to stop main thread message pump
	MessageBoxThread msgbox(text.str());

	switch (msgbox.askUser())
	{
		case IDRETRY:
		{
			DebugBreak();

			break;
		}
		case IDCANCEL:
		{
			TerminateProcess(GetCurrentProcess(), EXIT_FAILURE);

			break;
		}
	}
}

}