#pragma once

#include <chrono>
#include <print>

namespace DXX::Debug
{

	class Timer
	{

	private:

		std::chrono::high_resolution_clock::time_point m_StartTime{};

		bool m_IsStopped{};

	public:
		Timer()
			: m_StartTime{ std::chrono::high_resolution_clock::now() } 
		{
		}

		~Timer()
		{
			if(!m_IsStopped) Stop();
		}

		void Stop()
		{
			const auto endTime{ std::chrono::high_resolution_clock::now() };

			const auto duration{ std::chrono::duration_cast<std::chrono::microseconds>(endTime - m_StartTime) };

			std::println("Elapsed Time: {0}ms", duration.count());

			m_IsStopped = true;
		}
	};

}