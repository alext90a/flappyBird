#include "stdafx.h"
#include "HighScoreDialog.h"


HighScoreDialog::HighScoreDialog()
{
	mLines.reserve(Game::kHighscoreLines);
}


HighScoreDialog::~HighScoreDialog()
{
}

void HighScoreDialog::setScore(const std::string& name, int score)
{
	for (unsigned int i = 0; i < mLines.size(); ++i)
	{
		if (mLines[i]->getValue() < score)
		{
			for (unsigned int j = mLines.size()-1; j != i; --j)
			{
				mLines[j]->setValue(mLines[j - 1]->getName(), mLines[j - 1]->getValue());
			}
			mLines[i]->setValue(name, score);
			mLines[i]->setColor(mNewScoreColor);
			if (i == Game::kHighscoreLines - 1)
			{
				mMinScore = score;
			}
			mLastChangedLine = mLines[i];
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
		mLastChangedLine = nullptr;
	}
	mGameObject->setEnabled(false);
}
