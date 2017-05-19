#include "stdafx.h"
#include "HighScoreDialog.h"


HighScoreDialog::HighScoreDialog()
{
	mLines.reserve(kHighscoreLines);
}


HighScoreDialog::~HighScoreDialog()
{
}

void HighScoreDialog::setScore(const std::string& name, int score)
{
	for (int i = 0; i < mLines.size(); ++i)
	{
		if (mLines[i]->getValue() < score)
		{
			mLines[i]->setValue(name, score);
			mLines[i]->setColor(mNewScoreColor);
			if (i == kHighscoreLines - 1)
			{
				mMinScore = score;
			}
			break;
		}
	}
}

void HighScoreDialog::addHighScoreLine(HighscoreLine* line)
{
	mLines.push_back(line);
	if (mMinScore > line->getValue())
	{
		mMinScore = line->getValue();
	}
}

int HighScoreDialog::getMinScore()const
{
	return mMinScore;
}

void HighScoreDialog::showDialog()
{
	mGameObject->setEnabled(true);

}

void HighScoreDialog::showDialog(const std::string& name, const int score)
{
	if (mMinScore < score)
	{
		setScore(name, score);
	}
	mGameObject->setEnabled(true);
}

void HighScoreDialog::closeDialog()
{
	if (mLastChangedLine != nullptr)
	{
		mLastChangedLine->setColor(mNormalLineColor);
	}
}
