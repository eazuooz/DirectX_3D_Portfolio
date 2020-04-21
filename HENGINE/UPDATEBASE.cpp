#include "UPDATEBASE.h"

UpdateBase::UpdateBase() : m_IsUpdate(true), m_IsDeath(false)
{
}


UpdateBase::~UpdateBase()
{
}

void UpdateBase::UpdateAfter(){}
void UpdateBase::Update(){}
void UpdateBase::UpdateBefore(){}
void UpdateBase::FinalUpdate(){}
void UpdateBase::RenderBefore(){}
void UpdateBase::RenderAfter(){}
void UpdateBase::ColBefore(){}
void UpdateBase::ColAfter(){}
void UpdateBase::Release(){}
void UpdateBase::ReleaseAfter(){}