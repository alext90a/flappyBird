#pragma once
class HighscoreLine;
class HighScoreDialog : public Component
{
public:
	HighScoreDialog();
	~HighScoreDialog();
	void addHighScoreLine(HighscoreLine* scoreLine);	
	void showDialog();
	void showDialog(const std::string& name, const int score);
	void closeDialog();
protected:
	int getMinScore()const;
	void setScore(const std::string& name, int score);

protected:
	std::vector<HighscoreLine*> mLines;
	int mMinScore = 10000000;
	HighscoreLine* mLastChangedLine = nullptr;
	D3DCOLOR mNormalLineColor = D3DCOLOR_RGBA(255, 255, 255, 255);
	D3DCOLOR mNewScoreColor = D3DCOLOR_RGBA(0, 255, 0, 255);
};

