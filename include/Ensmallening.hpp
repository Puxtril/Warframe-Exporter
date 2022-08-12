#pragma once

namespace WarframeExporter
{
	struct Ensmallening {
	private:
		bool _postEnsmalleningPart1;
		bool _postEnsmalleningPart2;
		bool _postEnsmalleningPart3;

	public:
		Ensmallening(bool part1, bool part2, bool part3)
			: _postEnsmalleningPart1(part1),
			_postEnsmalleningPart2(part2),
			_postEnsmalleningPart3(part3)
		{}

		bool isPostPart1() const { return _postEnsmalleningPart1; }
		bool isPostPart2() const { return _postEnsmalleningPart2; }
		bool isPostPart3() const { return _postEnsmalleningPart3; }
	};
}