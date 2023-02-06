#include "pch.h"
#include "TestAnimation.h"
#include "Input.h"
#include "Animator.h"

void TestAnimation::Update()
{
	if (INPUT->GetButtonDown(KEY_TYPE::KEY_1))
	{
		int32 count = GetAnimator()->GetAnimCount();
		int32 currentIndex = GetAnimator()->GetCurrentClipIndex();

		int32 index = (currentIndex + 1) % count;
		GetAnimator()->Play(index);
	}
}