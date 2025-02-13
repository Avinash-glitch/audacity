/**********************************************************************

  Audacity: A Digital Audio Editor

  AutoDuck.h

  Markus Meyer

**********************************************************************/

#ifndef __AUDACITY_EFFECT_AUTODUCK__
#define __AUDACITY_EFFECT_AUTODUCK__

#include "Effect.h"

class wxBitmap;
class wxTextCtrl;
class EffectAutoDuckPanel;
class ShuttleGui;

#define AUTO_DUCK_PANEL_NUM_CONTROL_POINTS 5

class EffectAutoDuck final : public Effect
{
public:
   static const ComponentInterfaceSymbol Symbol;

   EffectAutoDuck();
   virtual ~EffectAutoDuck();

   // ComponentInterface implementation

   ComponentInterfaceSymbol GetSymbol() const override;
   TranslatableString GetDescription() const override;
   ManualPageID ManualPage() const override;

   // EffectDefinitionInterface implementation

   EffectType GetType() const override;
   bool GetAutomationParameters(CommandParameters & parms) const override;
   bool SetAutomationParameters(const CommandParameters & parms) override;

   // EffectProcessor implementation

   bool VisitSettings( SettingsVisitor & S ) override;

   // Effect implementation

   bool Init() override;
   void End() override;
   bool Process(EffectSettings &settings) override;
   std::unique_ptr<EffectUIValidator> PopulateOrExchange(
      ShuttleGui & S, EffectSettingsAccess &access) override;
   bool TransferDataToWindow(const EffectSettings &settings) override;
   bool DoTransferDataToWindow();

private:
   // EffectAutoDuck implementation

   bool ApplyDuckFade(int trackNum, WaveTrack *t, double t0, double t1);

   void OnValueChanged(wxCommandEvent & evt);

private:
   double mDuckAmountDb;
   double mInnerFadeDownLen;
   double mInnerFadeUpLen;
   double mOuterFadeDownLen;
   double mOuterFadeUpLen;
   double mThresholdDb;
   double mMaximumPause;

   const WaveTrack *mControlTrack;

   wxTextCtrl *mDuckAmountDbBox;
   wxTextCtrl *mInnerFadeDownLenBox;
   wxTextCtrl *mInnerFadeUpLenBox;
   wxTextCtrl *mOuterFadeDownLenBox;
   wxTextCtrl *mOuterFadeUpLenBox;
   wxTextCtrl *mThresholdDbBox;
   wxTextCtrl *mMaximumPauseBox;
   EffectAutoDuckPanel *mPanel;

   DECLARE_EVENT_TABLE()

   friend class EffectAutoDuckPanel;
};

class EffectAutoDuckPanel final : public wxPanelWrapper
{
public:
   EffectAutoDuckPanel(
      wxWindow *parent, wxWindowID winid, EffectAutoDuck *effect);
   virtual ~EffectAutoDuckPanel();

private:
   enum EControlPoint
   {
      innerFadeDown = 0,
      outerFadeDown,
      innerFadeUp,
      outerFadeUp,
      duckAmount,
      none = 99,
   };

   bool AcceptsFocus() const override { return false; }
   // So that wxPanel is not included in Tab traversal - see wxWidgets bug 15581
   bool AcceptsFocusFromKeyboard() const override { return false; }


   void OnPaint(wxPaintEvent & evt);
   void OnMouseCaptureChanged(wxMouseCaptureChangedEvent & evt);
   void OnMouseCaptureLost(wxMouseCaptureLostEvent & evt);
   void OnLeftDown(wxMouseEvent & evt);
   void OnLeftUp(wxMouseEvent & evt);
   void OnMotion(wxMouseEvent & evt);

   void ResetControlPoints();
   EControlPoint GetNearestControlPoint(const wxPoint & pt);

private:
   wxWindow *mParent;
   EffectAutoDuck *mEffect;
   std::unique_ptr<wxBitmap> mBackgroundBitmap;
   EControlPoint mCurrentControlPoint;
   wxPoint mControlPoints[AUTO_DUCK_PANEL_NUM_CONTROL_POINTS];
   wxPoint mMoveStartControlPoints[AUTO_DUCK_PANEL_NUM_CONTROL_POINTS];
   wxPoint mMouseDownPoint;
   bool mControlPointMoveActivated;

   DECLARE_EVENT_TABLE()
};

#endif
