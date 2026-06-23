#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "AtomTypes.h"
#include "AtomBase.generated.h"

class AAtomBase;

// 存在原子上的单条键记录
USTRUCT(BlueprintType)
struct FBondRecord
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "键")
    TWeakObjectPtr<AAtomBase> PartnerAtom;

    UPROPERTY(BlueprintReadOnly, Category = "键")
    EBondType BondType = EBondType::Single;

    // 本原子占用的槽位索引
    UPROPERTY(BlueprintReadOnly, Category = "键")
    int32 MySlotIndex = INDEX_NONE;

    // 对方原子占用的槽位索引
    UPROPERTY(BlueprintReadOnly, Category = "键")
    int32 PartnerSlotIndex = INDEX_NONE;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAtomProximityEnter, AAtomBase*, OtherAtom);

UCLASS(Abstract)
class CHEMICALBOND_API AAtomBase : public AActor
{
    GENERATED_BODY()

public:
    AAtomBase();

protected:
    virtual void BeginPlay() override;

    // -----------------------------------------------------------------------
    // 配置属性（在 Blueprint 子类的 Default 面板中设置）
    // -----------------------------------------------------------------------

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "原子|配置")
    EAtomElementType ElementType = EAtomElementType::C_Normal;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "原子|配置")
    UDataTable* AtomDataTable = nullptr;

    // 接近检测半径，TODO-0005 公式确认前使用固定值
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "原子|配置")
    float ProximityRadius = 200.f;

    // -----------------------------------------------------------------------
    // 运行时只读属性（BeginPlay 从 DataTable 填入）
    // -----------------------------------------------------------------------

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "原子|运行时")
    float Mass = 0.f;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "原子|运行时")
    int32 TotalSlots = 0;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "原子|运行时")
    bool bCanFormRing = false;

public:
    // -----------------------------------------------------------------------
    // 事件
    // -----------------------------------------------------------------------

    // 接近范围内出现其他原子时广播，由上层决策系统监听
    UPROPERTY(BlueprintAssignable, Category = "原子|事件")
    FOnAtomProximityEnter OnProximityEnter;

    // 状态变化时由 Blueprint 子类实现视觉反馈
    UFUNCTION(BlueprintImplementableEvent, Category = "原子|事件")
    void OnAtomStateChanged(EAtomState NewState);

    // -----------------------------------------------------------------------
    // Blueprint 可调用方法 — 查询
    // -----------------------------------------------------------------------

    UFUNCTION(BlueprintCallable, Category = "原子|查询")
    int32 GetAvailableSlotCount() const;

    // 返回第一个空闲槽位的索引，无空槽时返回 INDEX_NONE
    UFUNCTION(BlueprintCallable, Category = "原子|查询")
    int32 FindFreeSlotIndex() const;

    UFUNCTION(BlueprintCallable, Category = "原子|查询")
    float GetMass() const { return Mass; }

    UFUNCTION(BlueprintCallable, Category = "原子|查询")
    EAtomState GetAtomState() const { return AtomState; }

    UFUNCTION(BlueprintCallable, Category = "原子|查询")
    TArray<FBondRecord> GetBonds() const { return Bonds; }

    UFUNCTION(BlueprintCallable, Category = "原子|查询")
    EAtomElementType GetElementType() const { return ElementType; }

    // -----------------------------------------------------------------------
    // Blueprint 可调用方法 — 槽位与键操作
    // -----------------------------------------------------------------------

    // 双方都需要调用各自的 AddBond，由上层连接系统负责协调
    UFUNCTION(BlueprintCallable, Category = "原子|连接")
    bool AddBond(AAtomBase* Partner, EBondType InBondType, int32 MySlot, int32 PartnerSlot);

    UFUNCTION(BlueprintCallable, Category = "原子|连接")
    bool RemoveBond(int32 MySlotIndex);

    // -----------------------------------------------------------------------
    // Blueprint 可调用方法 — 状态
    // -----------------------------------------------------------------------

    UFUNCTION(BlueprintCallable, Category = "原子|状态")
    void SetAtomState(EAtomState NewState);

private:
    UPROPERTY()
    USphereComponent* ProximitySphere = nullptr;

    // 槽位占用状态，长度 = TotalSlots，由 InitFromDataTable 初始化
    UPROPERTY()
    TArray<bool> SlotOccupied;

    UPROPERTY()
    TArray<FBondRecord> Bonds;

    UPROPERTY()
    EAtomState AtomState = EAtomState::Free;

    void InitFromDataTable();

    UFUNCTION()
    void HandleProximitySphereOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);
};
